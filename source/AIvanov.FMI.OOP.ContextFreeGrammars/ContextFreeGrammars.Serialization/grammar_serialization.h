#pragma once
#include "stdafx.h"
#include <ostream>
#include <istream>
#include <string>
#include "../ContextFreeGrammars.Grammars/grammar.h"

std::ostream& operator <<(std::ostream& stream, const grammar &grammar);
std::ostream& operator <<(std::ostream& stream, const production &production);
std::ostream& operator <<(std::ostream& stream, const concatenation &concatenation);
std::ostream& operator <<(std::ostream& stream, const term &term);

bool try_deserialize_grammar(std::istream &text, grammar *outGrammar);
bool try_deserialize_grammar_header(std::istream &text, std::string *outStartingRule, int *outRuleCount);
bool try_deserialize_production_rule(const std::string &text, production *outRule);
bool try_deserialize_concatenation(const std::string &text, concatenation *outConcatenation);
bool try_deserialize_term(const std::string &text, term *outTerm);
bool try_deserialize_identifier(const std::string &text, std::string *outIdentifier);
bool try_get_word(std::istream &stream, const char *word);
bool try_deserialize_integer(const std::string &text, int *outValue);
