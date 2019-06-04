#pragma once
#include "../ContextFreeGrammars.Grammars/grammar.h"
#include "production_rule_deserializer.h"

bool try_deserialize_grammar(std::istream &stream, grammar *outGrammar);
bool try_deserialize_integer(const std::string &text, int *outValue);
bool try_deserialize_identifier(const std::string &text, std::string *outIdentifier);
bool try_deserialize_production_rule(const std::string &text, production *outProduction);