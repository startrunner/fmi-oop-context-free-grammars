#include "stdafx.h"
#include "informal_grammar_serialization.h"
using namespace std;

std::ostream& operator<<(std::ostream &stream, const grammar &grammar)
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
	if (production.operand_count() == 0)return stream << term::EMPTY_WORD;

	for (int i = 0; i < production.operand_count(); i++)
	{
		stream << production.operand(i);
		if (i + 1 < production.operand_count())stream << " | ";
	}

	return stream;
}

std::ostream& operator<<(std::ostream &stream, const concatenation &concatenation)
{
	if (concatenation.operand_count() == 0)return stream << term::EMPTY_WORD;
	for (int i = 0; i < concatenation.operand_count(); i++)
	{
		stream << concatenation.operand(i);
		if (i + 1 < concatenation.operand_count())stream << ".";
	}
	return stream;
}

std::ostream& operator<<(std::ostream &stream, const term &term)
{
	return stream<<term.value();
}