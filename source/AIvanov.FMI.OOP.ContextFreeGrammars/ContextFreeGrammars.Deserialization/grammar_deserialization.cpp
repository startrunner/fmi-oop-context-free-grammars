#include "stdafx.h"
#include <sstream>
#include "grammar_deserialization.h"
#include "../ContextFreeGrammars.Infrastructure/stringutil.h"
using namespace std;

bool try_deserialize_grammar(std::istream &stream, grammar *outGrammar)
{
	production_rule_deserializer deserializer{ &stream };

	vector<pair<string, production>> namedRules;

	string name;
	production rule;

	while (deserializer.try_get_next_named(&name, &rule))
	{
		namedRules.emplace_back(name, rule);
	}

	if (namedRules.empty() || !stream.eof())return false;

	if (outGrammar != nullptr)
	{
		*outGrammar = grammar{};
		for (pair<string, production> &namedRule : namedRules)
		{
			outGrammar->add_rule(namedRule.first, namedRule.second);
		}
		outGrammar->set_start_symbol(namedRules.front().first);
	}
	return true;
}

bool try_deserialize_identifier(const string &text, string *outIdentifier)
{
	string trimmed = trim(text, ' ');

	if (!term::validate_value(trimmed))return false;

	if (outIdentifier != nullptr)*outIdentifier = trimmed;
	return true;
}

bool try_deserialize_production_rule(const std::string & text, production *outProduction)
{
	istringstream stream{ text };
	production_rule_deserializer deserializer{ &stream };

	production rule;
	if (!deserializer.try_get_next(&rule) || !stream.eof())return false;

	if (outProduction != nullptr)*outProduction = rule;
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