#include "production_parsing.h"
#include "../ContextFreeGrammars.Grammars/production.h"
using namespace std;

production parse_production(int firstIndex, int lastIndex)
{
	vector<int> choiceIndices = find_indices(firstIndex, lastIndex, token_type::choice);

	if (choiceIndices.empty())return parse_concatenation(firstIndex, lastIndex);

	production result;

	result.add_operand(parse_concatenation(firstIndex, choiceIndices.front() - 1));

	for (int i = 1; i < choiceIndices.size(); i++)
	{
		int currentFirst = choiceIndices[i - 1] + 1;
		int currentLast = choiceIndices[i] - 1;
		result.add_operand(parse_concatenation(currentFirst, currentLast));
	}

	result.add_operand(parse_concatenation(choiceIndices.back() + 1, lastIndex));

	return result;
}

vector<int> production_rule_deserializer::find_indices(int firstIndex, int lastIndex, token_type type)
{
	vector<int> indices;

	for (int i = firstIndex; i <= lastIndex; i++)
	{
		if (tokenizer.token_at(i).type() == type)indices.push_back(i);
	}

	return indices;
}

concatenation production_rule_deserializer::parse_concatenation(int firstIndex, int lastIndex)
{
	concatenation result;

	for (int i = firstIndex; i <= lastIndex; i += 2)
	{
		token token = tokenizer.token_at(i);

		term operand;
		if (token.type() == token_type::terminal)operand = term{ token.value(), term_type::terminal };
		else operand = term{ token.value(), term_type::rule_reference };

		result.add_operand(operand);
	}

	return result;
}