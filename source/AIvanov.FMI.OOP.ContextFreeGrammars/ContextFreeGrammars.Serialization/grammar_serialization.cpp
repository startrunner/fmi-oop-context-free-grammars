#include "stdafx.h"
#include "grammar_serialization.h"
using namespace std;

std::ostream &serialize_grammar(std::ostream &stream, const grammar &grammar)
{
	vector<string> ruleNames = grammar.get_rule_names();
	string startSymbol = grammar.start_symbol();

	serialize_named_rule(stream, startSymbol, grammar.get_rule(startSymbol));

	for (string &ruleName : ruleNames)
	{
		if (ruleName == startSymbol)continue;
		serialize_named_rule(stream, ruleName, grammar.get_rule(ruleName));
	}

	return stream;
}

std::ostream &serialize_named_rule(std::ostream &stream, const std::string &name, const production &rule)
{
	stream << name << " -> ";
	serialize_rule(stream, rule);
	return stream << "; ";
}

std::ostream &serialize_rule(std::ostream &stream, const production &rule)
{
	for (int i = 0; i < rule.operand_count(); i++)
	{
		serialize_concatenation(stream, rule.operand(i));
		if (i + 1 < rule.operand_count())stream << " | ";
	}

	return stream;
}

std::ostream &serialize_concatenation(std::ostream &stream, const concatenation &concatenation)
{
	for (int i = 0; i < concatenation.operand_count(); i++)
	{
		serialize_production_term(stream, concatenation.operand(i));
		if (i + 1 < concatenation.operand_count())stream << '.';
	}

	return stream;
}

std::ostream &serialize_production_term(std::ostream &stream, const term &term)
{
	return stream << term.value();
}
