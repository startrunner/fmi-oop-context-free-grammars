#include "stdafx.h"
#include "grammar.h"
#include "../ContextFreeGrammars.Infrastructure/stringutil.h"
using namespace std;

bool grammar::try_match(const string &word, cyk_parse_tree *outParseTree) const
{
	vector<string> splitWord = split(word, ' ');
	return try_match(splitWord, outParseTree);
}

bool grammar::try_match(const vector<string> &word, cyk_parse_tree *outParseTree) const
{
	if (!is_chomskified())throw std::runtime_error("Cannot run CYK algorithm on grammar that is not in Chomsky normal form.");

	if (word.empty())return is_directly_nullable(get_rule(start_symbol()));

	//cykTable[firstCharacter][charSpan] := (mapping $rule->(cyk parse tree for char span with $rule))
	vector<vector<map<string, cyk_parse_tree_node>>> cykTable = initialize_cyk_table(word);

	const vector<binary_concatenation_rule> concatenationRules = get_binary_concatenation_rules();
	for (int length = 2; length <= word.size(); length++)
	{
		for (int start = 0; start < word.size() - length + 1; start++)
		{
			for (int leftLength = 1; leftLength < length; leftLength++)
			{
				const pair<int, int>
					leftSpan = { start, leftLength },
					rightSpan = { start + leftLength, length - leftLength };

				for (const binary_concatenation_rule &rule : concatenationRules)
				{
					bool eligible =
						cykTable[leftSpan.first][leftSpan.second].count(rule.leftOperand) != 0 &&
						cykTable[rightSpan.first][rightSpan.second].count(rule.rightOperand) != 0;

					if (eligible)
					{
						cyk_parse_tree_node node{ rule.ruleName };
						node.add_child(cykTable[leftSpan.first][leftSpan.second][rule.leftOperand]);
						node.add_child(cykTable[rightSpan.first][rightSpan.second][rule.rightOperand]);
						cykTable[start][length][rule.ruleName] = node;
					}
				}
			}
		}
	}

	bool success = cykTable[0][word.size()].size() != 0;

	if (success && outParseTree != nullptr)
	{
		*outParseTree = cyk_parse_tree{ cykTable[0][word.size()].begin()->second };
	}

	return success;
}

std::vector<grammar::binary_concatenation_rule> grammar::get_binary_concatenation_rules()const
{
	vector<binary_concatenation_rule> rules;

	vector<string> ruleNames = get_rule_names();

	for (string &name : ruleNames)
	{
		const production &rule = get_rule(name);

		for (int i = 0; i < rule.operand_count(); i++)
		{
			const concatenation &concatenation = rule.operand(i);
			bool eligible =
				concatenation.operand_count() == 2 &&
				!concatenation.operand(0).is_terminal() &&
				!concatenation.operand(1).is_terminal();
			if (eligible)
			{
				rules.emplace_back(name, concatenation.operand(0).value(), concatenation.operand(1).value());
			}
		}
	}

	return rules;
}

vector<vector<map<string, cyk_parse_tree_node>>> grammar::initialize_cyk_table(const vector<string> &word) const
{
	vector<vector<map<string, cyk_parse_tree_node>>> cykTable;

	cykTable.resize(word.size());
	for (int i = 0; i < word.size(); i++)cykTable[i].resize(word.size() - i + 1);

	const vector<string> ruleNames = get_rule_names();

	for (int charI = 0; charI < word.size(); charI++)
	{
		const string &character = word[charI];
		for (const string &ruleName : ruleNames)
		{
			const production &rule = get_rule(ruleName);
			for (int i = 0; i < rule.operand_count(); i++)
			{
				const concatenation &concatenation = rule.operand(i);

				bool eligible =
					concatenation.operand_count() == 1 &&
					concatenation.operand(0).is_terminal() &&
					concatenation.operand(0).value() == character;

				if (eligible)cykTable[charI][1][ruleName] = cyk_parse_tree_node{
					ruleName,
					{cyk_parse_tree_node{concatenation.operand(0).value()} }
				};
			}
		}
	}

	return cykTable;
}