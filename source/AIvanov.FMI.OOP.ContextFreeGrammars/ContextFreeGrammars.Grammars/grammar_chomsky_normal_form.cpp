#include "grammar.h"
#include "stdafx.h"
#include "grammar.h"
#include "../ContextFreeGrammars.Infrastructure/bitmaskutil.h"
using namespace std;

string grammar::wrap_start_symbol() { return wrap_start_symbol(this->startSymbol); }
string grammar::wrap_start_symbol(const string &suggestedName)
{
	string newStartSymbol = mangle_name(suggestedName);

	term term{ startSymbol, term_type::rule_reference };

	productionRules.emplace(newStartSymbol, term);
	startSymbol = newStartSymbol;

	return newStartSymbol;
}

grammar &grammar::operator=(const grammar &toAssign)
{
	if (this == &toAssign)return *this;
	startSymbol = toAssign.startSymbol;
	productionRules = toAssign.productionRules;
	return *this;
}

bool grammar::operator==(const grammar & other) const
{
	return startSymbol == other.startSymbol && productionRules == other.productionRules;
}

bool grammar::try_chomskify()
{
	if (is_chomskified())return true;

	wrap_start_symbol();
	eliminate_nonsolitary_terminals();
	eliminate_long_concatenations();
	inline_nullable_rules();
	inline_unit_rules();
	remove_unreferenced_rules();
	sort_production_operands();

	return is_chomskified();
}

void grammar::eliminate_nonsolitary_terminals()
{
	vector<std::string> oldRuleNames = get_rule_names();

	for (const string &name : oldRuleNames)
	{
		production &rule = this->get_rule(name);
		;
		for (int i = 0; i < rule.operand_count(); i++)
		{
			concatenation &operand = rule.operand(i);
			eliminate_nonsolitary_terminals(name, operand);
		}
	}
}

term grammar::create_terminal_rule(const string &ownerName, const string &terminalValue)
{
	string suggestedName = "T_" + terminalValue;
	string name = mangle_name(suggestedName);
	add_rule(name, terminalValue);
	return term{ name, term_type::rule_reference };
}

void grammar::eliminate_long_concatenations()
{
	vector<string> ruleNames = get_rule_names();

	for (string &name : ruleNames)
	{
		production &rule = get_rule(name);

		for (int i = 0; i < rule.operand_count(); i++)
		{
			concatenation &operand = rule.operand(i);
			eliminate_long_concatenations(name, operand);
		}
	}
}

void grammar::inline_unit_rules()
{
	set<pair<string, string>> done;

	bool any = true;
	while (any)
	{
		any = false;
		vector<string> ruleNames = get_rule_names();
		for (string &name : ruleNames)
		{
			production &rule = get_rule(name);
			set<int> replacedIndices;
			vector<concatenation> substitutes;

			for (int i = 0; i < rule.operand_count(); i++)
			{
				concatenation &concatenation = rule.operand(i);

				bool eligible =
					concatenation.operand_count() == 1 &&
					!concatenation.operand(0).is_terminal() &&
					done.count({ name, concatenation.operand(0).value() }) == 0;

				if (eligible)
				{
					done.insert({ name, concatenation.operand(0).value() });

					string substituteName = concatenation.operand(0).value();
					any = true;

					production &substitute = get_rule(concatenation.operand(0).value());
					for (int j = 0; j < substitute.operand_count(); j++)
					{
						substitutes.push_back(substitute.operand(j));
					}
					replacedIndices.insert(i);
				}
			}

			;
			rule.remove_operands(replacedIndices);
			rule.add_operands(substitutes);

			remove_repeating_concatenations(rule);
		}
	}
}

void grammar::eliminate_long_concatenations(const string &ruleName, concatenation &rule)
{
	if (rule.operand_count() <= 2)return;

	vector<term> operands;
	operands.reserve(rule.operand_count());
	for (int i = 0; i < rule.operand_count(); i++)operands.push_back(rule.operand(i));

	rule = create_binary_concatenation(ruleName, operands);
}

concatenation grammar::create_binary_concatenation(const string &ruleName, vector<term> operands)
{
	while (operands.size() > 2)
	{
		term right = operands.back();
		operands.pop_back();

		term left = operands.back();
		operands.pop_back();

		term concatenationRule = create_concatenation_rule(ruleName, left, right);
		operands.push_back(concatenationRule);
	}

	concatenation result;
	for (term &operand : operands)result.add_operand(operand);
	return result;
}

term grammar::create_concatenation_rule(const std::string &parentRuleName, const term &left, const term &right)
{
	string suggestedName = "C_" + parentRuleName;
	string name = mangle_name(suggestedName);

	concatenation concatenation;
	concatenation.add_operand(left);
	concatenation.add_operand(right);

	add_rule(name, concatenation);

	return term{ name, term_type::rule_reference };
}

vector<concatenation> grammar::substitute_directly_nullable_rules(
	const concatenation &replaceIn,
	const set<string> &directlyNullables
)
{
	vector<concatenation> substitutes;

	vector<int> directlyNullableOperands =
		get_directly_nullable_operand_indices(replaceIn, directlyNullables);

	vector<bool> operandBitmask = vector<bool>(directlyNullableOperands.size(), false);

	map<int, int> operandIndicesInBitmask;
	for (int i = 0; i < directlyNullableOperands.size(); i++)
	{
		operandIndicesInBitmask[directlyNullableOperands[i]] = i;
	}

	do
	{
		const bool isCurrentNullable =
			operandBitmask.size() == replaceIn.operand_count() &&
			is_all_false(operandBitmask);

		concatenation current;

		for (int i = 0; i < replaceIn.operand_count(); i++)
		{
			bool operandIncluded =
				replaceIn.operand(i).is_terminal() ||
				operandIndicesInBitmask.count(i) == 0 ||
				operandBitmask[operandIndicesInBitmask[i]] == true;

			if (operandIncluded)current.add_operand(replaceIn.operand(i));
		}

		if (current.operand_count() == 0)current = term{}.make_empty_word();

		substitutes.push_back(current);
	} while (try_advance_bitmask(operandBitmask));

	return substitutes;
}

void grammar::remove_repeating_concatenations(production &rule)
{
	set<concatenation> concatenations;
	set<int> duplicatingIndices;

	for (int i = 0; i < rule.operand_count(); i++)
	{
		concatenation &operand = rule.operand(i);

		if (concatenations.count(operand) != 0)duplicatingIndices.insert(i);
		else concatenations.insert(operand);
	}

	rule.remove_operands(duplicatingIndices);
}

void grammar::inline_nullable_rules()
{
	set<string> ignoredNullables = { start_symbol() };

	set<string> newNullables;

	while (!(newNullables = get_directly_nullable_rules(ignoredNullables)).empty())
	{
		inline_directly_nullable_rules(newNullables);
		for (const string &nullable : newNullables)ignoredNullables.insert(nullable);
	}

	vector<string> ruleNames = get_rule_names();
	for (string &name : ruleNames)
	{
		if (name == start_symbol())continue;

		make_non_directly_nullable(name);
	}
}

void grammar::inline_directly_nullable_rules(const set<std::string>& toInline)
{
	vector<string> ruleNames = get_rule_names();
	for (string &name : ruleNames)
	{
		production &rule = get_rule(name);

		set<int> substitutedIndices;
		vector<concatenation> substitutes;

		for (int i = 0; i < rule.operand_count(); i++)
		{
			concatenation &ruleVariant = rule.operand(i);

			vector<concatenation> currentSubstitutes = substitute_directly_nullable_rules(ruleVariant, toInline);
			;
			if (currentSubstitutes.size() > 1)
			{
				substitutedIndices.insert(i);
				for (concatenation &substitute : currentSubstitutes)substitutes.push_back(substitute);
			}
		}

		if (!substitutedIndices.empty())
		{
			rule.remove_operands(substitutedIndices);
			rule.add_operands(substitutes);
			remove_repeating_concatenations(rule);
		}
	}
}

set<string> grammar::get_directly_nullable_rules(const set<string> &toIgnore)const
{
	set<string> result;

	vector<string> names = get_rule_names();
	for (string &name : names)
	{
		if (toIgnore.find(name) != toIgnore.end())continue;

		const production &rule = get_rule(name);
		if (is_directly_nullable(rule))result.insert(name);
	}

	return result;
}

void grammar::make_non_directly_nullable(const string &ruleName)
{
	production &rule = get_rule(ruleName);
	set<int> nullIndices;

	for (int i = 0; i < rule.operand_count(); i++)
	{
		if (is_directly_nullable(rule.operand(i)))nullIndices.insert(i);
	}

	rule.remove_operands(nullIndices);
}

bool grammar::is_directly_nullable(const production &rule)
{
	bool result = false;
	for (int i = 0; !result && i < rule.operand_count(); i++)
	{
		result |= is_directly_nullable(rule.operand(i));
	}
	return result;
}

bool grammar::is_directly_nullable(const concatenation &rule)
{
	bool result = true;
	for (int i = 0; result && i < rule.operand_count(); i++)
	{
		result &= rule.operand(i).is_empty_word();
	}
	return result;
}

vector<int> grammar::get_directly_nullable_operand_indices(const concatenation &concatenation, const set<string> &directlyNullables)
{
	vector<int> indices;

	for (int i = 0; i < concatenation.operand_count(); i++)
	{
		const term &operand = concatenation.operand(i);
		bool eligible = !operand.is_terminal() && directlyNullables.count(operand.value()) != 0;
		if (eligible)indices.push_back(i);
	}

	return indices;
}

void grammar::remove_unreferenced_rules()
{
	set<string> usedRules = get_referenced_rules();
	vector<string> allRules = get_rule_names();

	for (string &name : allRules)
	{
		if (usedRules.count(name) == 0)remove_rule(name);
	}
}

set<string> grammar::get_referenced_rules()const
{
	set<string> referencedRules{ start_symbol() };

	const vector<string> ruleNames = get_rule_names();

	for (string name : ruleNames)
	{
		const production &rule = get_rule(name);
		for (int i = 0; i < rule.operand_count(); i++)
		{
			const concatenation &concatenation = rule.operand(i);
			for (int j = 0; j < concatenation.operand_count(); j++)
			{
				const term &term = concatenation.operand(j);
				if (!term.is_terminal())referencedRules.insert(term.value());
			}
		}
	}

	return referencedRules;
}

void grammar::sort_production_operands()
{
	vector<string> ruleNames = get_rule_names();

	for (string &name : ruleNames)
	{
		get_rule(name).sort_operands();
	}
}

bool grammar::symbols_exist(const production &rule) const
{
	for (int i = 0; i < rule.operand_count(); i++)
	{
		const concatenation &concatenation = rule.operand(i);

		for (int j = 0; j < concatenation.operand_count(); j++)
		{
			term term = concatenation.operand(j);
			if (!term.is_terminal() && productionRules.count(term.value()) == 0)return false;
		}
	}

	return true;
}

bool grammar::is_chomskified()const
{
	vector<string> ruleNames = get_rule_names();

	for (string &name : ruleNames)
	{
		const production &rule = get_rule(name);

		const bool isStartSymbol = name == start_symbol();

		for (int i = 0; i < rule.operand_count(); i++)
		{
			const concatenation &concatenation = rule.operand(i);

			if (!isStartSymbol)
			{
				for (int j = 0; j < concatenation.operand_count(); j++)
				{
					if (concatenation.operand(j).is_empty_word())
					{
						return false;
					}
				}
			}

			if (concatenation.operand_count() == 1)
			{
				if (!concatenation.operand(0).is_terminal())
				{
					return false;
				}
			}
			else if (concatenation.operand_count() == 2)
			{
				if (concatenation.operand(0).is_terminal())
				{
					return false;
				}
				if (concatenation.operand(1).is_terminal())
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}