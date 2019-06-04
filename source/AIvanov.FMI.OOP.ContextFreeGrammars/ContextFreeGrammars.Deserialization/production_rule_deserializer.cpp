#include "stdafx.h"
#include "production_rule_deserializer.h"
using namespace std;

bool production_rule_deserializer::filter_token_no_whitespace(const token &token)
{
	return token.type() != token_type::whitespace;
}

void production_rule_deserializer::refeed(std::istream *stream)
{
	tokenizer.refeed(stream);
}

bool production_rule_deserializer::try_get_next_named(string *outName, production *outRule)
{
	string name;
	int expressionStartIndex, expressionSpan, totalSpan;

	if (!try_peek_next_named(&name, &expressionStartIndex, &expressionSpan, &totalSpan))
	{
		return false;
	}

	int lastExpressionIndex = expressionStartIndex + expressionSpan - 1;

	production rule = parse_production(expressionStartIndex, lastExpressionIndex);

	if (outName)*outName = name;
	if (outRule)*outRule = rule;

	tokenizer.pop(totalSpan);
	return true;
}

bool production_rule_deserializer::try_get_next(production *outRule)
{
	int semicolonIndex = index_of_or_negative(token_type::semicolon);
	int arrowIndex = index_of_or_negative(token_type::arrow);

	int lastIndex;
	if (semicolonIndex == -1 && arrowIndex == -1)lastIndex = tokenizer.cache_all() - 1;
	else if (semicolonIndex == -1)lastIndex = arrowIndex - 2;
	else if (arrowIndex == -1)lastIndex = semicolonIndex - 1;
	else lastIndex = min(arrowIndex - 2, semicolonIndex - 1);

	int firstIndex = 0;

	if (!validate_expression(firstIndex, lastIndex))return false;
	production rule = parse_production(firstIndex, lastIndex);

	if (outRule != nullptr)*outRule = rule;
	return true;
}

bool production_rule_deserializer::try_peek_next_named(
	string *outName,
	int *outExpressionStartIndex,
	int *outExpressionSpan,
	int *outSpan
)
{
	if (!tokenizer.try_cache(4))return false;

	if (tokenizer.token_at(0).type() != token_type::non_terminal)return false;
	if (tokenizer.token_at(1).type() != token_type::arrow)return false;

	const int arrowIndex = 1;
	int semicolonIndex = index_of_or_negative(token_type::semicolon);
	if (!(semicolonIndex > arrowIndex + 1))return false;

	if (tokenizer.token_at(0).type() != token_type::non_terminal)return false;

	int firstExpressionIndex = arrowIndex + 1;
	int lastExpressionIndex = semicolonIndex - 1;

	if (!validate_expression(firstExpressionIndex, lastExpressionIndex))return false;

	if (outName)*outName = tokenizer.token_at(0).value();
	if (outExpressionStartIndex)*outExpressionStartIndex = firstExpressionIndex;
	if (outExpressionSpan)*outExpressionSpan = lastExpressionIndex - firstExpressionIndex + 1;
	if (outSpan)*outSpan = semicolonIndex - arrowIndex + 2;
	return true;
}

int production_rule_deserializer::index_of_or_negative(token_type type)
{
	for (int i = 0; tokenizer.try_cache(i + 1); i++)
	{
		if (tokenizer.token_at(i).type() == type)return i;
	}

	return -1;
}


production production_rule_deserializer::parse_production(int firstIndex, int lastIndex)
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

bool production_rule_deserializer::validate_expression(int firstIndex, int lastIndex)
{
	if (lastIndex < firstIndex)return false;

	for (int i = firstIndex; i <= lastIndex; i += 2)
	{
		token_type type = tokenizer.token_at(i).type();
		if (type != token_type::terminal && type != token_type::non_terminal)return false;
	}

	for (int i = firstIndex + 1; i <= lastIndex; i += 2)
	{
		token_type type = tokenizer.token_at(i).type();
		if (type != token_type::choice && type != token_type::concatenation)return false;
	}

	return true;
}

concatenation production_rule_deserializer::parse_concatenation(int firstIndex, int lastIndex)
{
	concatenation result;

	for (int i = firstIndex; i <= lastIndex; i += 2)
	{
		token token = tokenizer.token_at(i);

		int valueStartIndex = 0;

		term operand;
		if (token.type() == token_type::terminal)operand = term{ token.value(), term_type::terminal };
		else operand = term{ token.value(), term_type::rule_reference };

		result.add_operand(operand);
	}

	return result;
}

