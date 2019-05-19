#include "stdafx.h"
#include "grammar_serialization.h"
#include "../ContextFreeGrammars.Infrastructure/stringutil.h"
#include <iostream>
using namespace std;

std::ostream & operator<<(std::ostream &stream, const grammar &grammar)
{
	stream
		<< "grammar from " << grammar.start_symbol()
		<< " with " << grammar.rule_count() << " rules"
		<< endl;

	vector<string> ruleNames = grammar.get_rule_names();
	for (string &ruleName : ruleNames)
	{
		stream << ruleName << " -> " << grammar.get_rule(ruleName) << endl;
	}

	return stream;
}

std::ostream& operator<<(std::ostream &stream, const production &production)
{
	if (production.operand_count() == 0)return stream << "_";

	for (int i = 0; i < production.operand_count(); i++)
	{
		stream << production.operand(i);
		if (i + 1 < production.operand_count())stream << " | ";
	}

	return stream;
}

std::ostream& operator<<(std::ostream &stream, const concatenation &concatenation)
{
	if (concatenation.operand_count() == 0)return stream << "_";
	for (int i = 0; i < concatenation.operand_count(); i++)
	{
		stream << concatenation.operand(i);
		if (i + 1 < concatenation.operand_count())stream << ".";
	}
	return stream;
}

std::ostream& operator<<(std::ostream &stream, const term &term)
{
	if (term::determine_term_type(term.value()) == term.type())
	{
		return stream << term.value();
	}
	else
	{
		return stream << (char)term.type() << "$" << term.value();
	}
}

bool try_deserialize_grammar(istream &text, grammar *outGrammar)
{
	string startSymbol;
	int ruleCount;

	if (!try_deserialize_grammar_header(text, &startSymbol, &ruleCount))return false;

	grammar result;
	result.set_start_symbol(startSymbol);

	for (int i = 0; i < ruleCount; i++)
	{
		string ruleName;
		string ruleNameText;
		text >> ruleNameText;
		if (!text.good())return false;
		if (!try_deserialize_identifier(ruleNameText, &ruleName))return false;
		if (result.contains_rule(ruleName))return false;

		if (!try_get_word(text, "->"))return false;

		production rule;
		string ruleExpressionText;
		getline(text, ruleExpressionText);
		if (!text.good())return false;
		if (!try_deserialize_production_rule(ruleExpressionText, &rule))return false;

		result.add_rule(ruleName, rule);
	}

	if (outGrammar != nullptr)*outGrammar = result;
	return true;
}

bool try_deserialize_grammar_header(istream &text, string *outStartSymbol, int *outRuleCount)
{
	if (!try_get_word(text, "grammar"))return false;
	if (!try_get_word(text, "from"))return false;

	string startSymbol;
	string startSymbolText;
	text >> startSymbolText;
	if (!text.good())return false;
	if (!try_deserialize_identifier(startSymbolText, &startSymbol))return false;

	if (!try_get_word(text, "with"))return false;

	int ruleCount;
	string ruleCountText;
	text >> ruleCountText;
	if (!text.good())return false;
	if (!try_deserialize_integer(ruleCountText, &ruleCount))return false;

	if (!try_get_word(text, "rules"))return false;

	if (outRuleCount != nullptr)*outRuleCount = ruleCount;
	if (outStartSymbol != nullptr)*outStartSymbol = startSymbol;
	return true;
}

bool try_get_word(istream &stream, const char *word)
{
	string candidate;
	stream >> candidate;
	return stream.good() && candidate == word;
}

bool try_deserialize_production_rule(const string &text, production *outRule)
{
	vector<string> split = split_trim(text, '|', ' ');

	production result;

	for (string &operandText : split)
	{
		concatenation operand;
		if (!try_deserialize_concatenation(operandText, &operand))return false;

		result.add_operand(operand);
	}

	if (outRule != nullptr)*outRule = result;
	return true;
}

bool try_deserialize_concatenation(const string &text, concatenation *outConcatenation)
{
	vector<string> split = split_trim(text, '.', ' ');

	concatenation result;

	for (string &operandText : split)
	{
		term operand;
		if (!try_deserialize_term(operandText, &operand))return false;

		result.add_operand(operand);
	}

	if (outConcatenation != nullptr)*outConcatenation = result;
	return true;
}

bool try_deserialize_term(const string &text, term *outTerm)
{
	if (!string_contains(text, '$'))
	{
		string trimmed = trim(text, ' ');

		if (!term::validate_value(trimmed))return false;
		*outTerm = term{ trimmed };
		return true;
	}

	vector<string> split = split_trim(text, '$', ' ');

	if (split.size() != 2)return false;
	if (split.front().size() != 1)return false;
	if (split.front().front() != 't' && split.front().front() != 'r')return false;
	if (!term::validate_value(split.back()))return false;

	if (outTerm != nullptr)*outTerm = term{ split.back(), (term_type)split.front().front() };
	return true;
}

bool try_deserialize_identifier(const string &text, string *outIdentifier)
{
	string trimmed = trim(text, ' ');

	if (!term::validate_value(trimmed))return false;

	if (outIdentifier != nullptr)*outIdentifier = trimmed;
	return true;
}

bool try_deserialize_integer(const std::string &text, int *outValue)
{
	if (text.empty())return false;
	string trimmed = trim(text, ' ');

	int result = 0;

	for (char digit : trimmed)
	{
		if (!isdigit(digit))return false;
		result = result * 10 + digit - '0';
	}

	if (outValue != nullptr)*outValue = result;
	return true;
}