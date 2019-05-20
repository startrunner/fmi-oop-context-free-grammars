#include "grammar.h"
#include "stdafx.h"
#include "grammar.h"
#include "../ContextFreeGrammars.Infrastructure/stringutil.h"
#include "../ContextFreeGrammars.Infrastructure/bitmaskutil.h"
#include "cyk_parse_tree.h"
using namespace std;


vector<string> grammar::get_rule_names() const
{
	vector<string> ruleNames;
	ruleNames.reserve(productionRules.size());
	for (auto entry : productionRules)ruleNames.push_back(entry.first);
	return ruleNames;
}
int grammar::rule_count() const { return productionRules.size(); }

bool grammar::contains_rule(const string &ruleName) const { return productionRules.count(ruleName) != 0; }

const string &grammar::start_symbol() const { return startSymbol; }
void grammar::set_start_symbol(const string &startSymbol) { this->startSymbol = startSymbol; }

void grammar::add_rule(const string &ruleName, const production &expression) { productionRules.emplace(ruleName, expression); }
const production &grammar::get_rule(const string &ruleName) const { return productionRules.at(ruleName); }

production& grammar::get_rule(const string &ruleName) 
{
	production &rule = productionRules.at(ruleName);

	if (abs(rule.operand_count()) > 7)
	{

	}

	return rule;
}

void grammar::remove_rule(const string &ruleName) { productionRules.erase(ruleName); }

void prefix_rule_terms(production &production, const string &prefix)
{
	for (int i = 0; i < production.operand_count(); i++)
	{
		concatenation &concatenation = production.operand(i);
		for (int j = 0; j < concatenation.operand_count(); j++)
		{
			term &term = concatenation.operand(j);
			if (!term.is_terminal())
			{
				term.set_value(prefix + term.value());
			}
		}
	}
}

void add_rule_with_prefix(grammar &addTo, const string &ruleName, const production &rule, const string &prefix)
{
	production newRule = rule;
	prefix_rule_terms(newRule, prefix);
	addTo.add_rule(prefix + ruleName, newRule);
}

void add_rules_with_prefix(grammar &addTo, const grammar &addFrom, const std::string &prefix)
{
	vector<string> sourceRules = addFrom.get_rule_names();
	for (string &name : sourceRules)
	{
		const production &sourceRule = addFrom.get_rule(name);
		add_rule_with_prefix(addTo, name, sourceRule, prefix);
	}
}

template<typename TJoin> grammar join_grammars(const grammar &left, const grammar &right)
{
	string leftStart = "L" + left.start_symbol();
	string rightStart = "R" + right.start_symbol();

	grammar result;

	TJoin startRule;
	startRule.add_operand(term{ leftStart, term_type::rule_reference });
	startRule.add_operand(term{ rightStart, term_type::rule_reference });

	result.add_rule("Start", startRule);
	add_rules_with_prefix(result, left, "L");
	add_rules_with_prefix(result, right, "R");

	return result;
}

grammar grammar::union_with(const grammar &other) const
{
	return join_grammars<production>(*this, other);
}

grammar grammar::concatenation_with(const grammar & other) const
{
	return join_grammars<concatenation>(*this, other);
}

grammar grammar::kleene_star() const
{
	grammar newGrammar = *this;

	string startSymbolName = newGrammar.mangle_name("Start");

	production startSymbol;

	concatenation concatenation;
	concatenation.add_operand(term{ startSymbolName, term_type::rule_reference });
	concatenation.add_operand(term{ start_symbol(), term_type::rule_reference });

	startSymbol.add_operand(term{ start_symbol(), term_type::rule_reference });
	startSymbol.add_operand(concatenation);
	startSymbol.add_operand(term{}.make_empty_word());

	newGrammar.add_rule(startSymbolName, startSymbol);

	return newGrammar;
}

bool grammar::is_empty() const
{
	set<string> walked{ start_symbol() };
	return !can_reach_terminal(get_rule(start_symbol()), walked);
}

bool grammar::can_reach_terminal(const production &from, set<string> &walkedRules)const
{
	bool success = false;

	for (int i = 0; !success && i < from.operand_count(); i++)
	{
		concatenation concatenation = from.operand(i);
		success |= can_reach_terminal(concatenation, walkedRules);
	}

	return success;
}

bool grammar::can_reach_terminal(const concatenation &from, set<string> &walkedRules) const
{
	bool success = false;

	for (int i = 0; !success && i < from.operand_count(); i++)
	{
		term term = from.operand(i);
		if (term.is_terminal())success = true;
		else if (walkedRules.count(term.value()) == 0)
		{
			walkedRules.insert(term.value());
			success |= can_reach_terminal(get_rule(term.value()), walkedRules);
		}
	}

	return success;
}

string grammar::mangle_name(const string &suggestedName, int startFrom)
{
	int mangleIndex = 1;
	string mangled;

	do
	{
		mangled = suggestedName + "_" + to_string(mangleIndex);
		mangleIndex++;
	} while (productionRules.find(mangled) != productionRules.end());

	return mangled;
}
