#pragma once
#include "grammar.h"

term operator ""_term(const char *literal, size_t length);
term operator ""_terminal(const char *literal, size_t length);
term operator ""_nonterminal(const char *literal, size_t length);
concatenation operator*(const term &left, const term &right);
concatenation operator*(const concatenation &left, const term &right);
production operator|(const concatenation &left, const concatenation &right);
production operator|(const production &left, const concatenation &right);
production operator|(const term &left, const concatenation &right);