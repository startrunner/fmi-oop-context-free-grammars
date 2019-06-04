#pragma once
#include "../ContextFreeGrammars.Grammars/grammar.h"

std::ostream& serialize_grammar(std::ostream &stream, const grammar &grammar);

std::ostream& serialize_named_rule(std::ostream &stream, const std::string &name, const production &rule);
std::ostream& serialize_rule(std::ostream &stream, const production &rule);
std::ostream& serialize_concatenation(std::ostream &stream, const concatenation &concatenation);
std::ostream& serialize_production_term(std::ostream &stream, const term &term);