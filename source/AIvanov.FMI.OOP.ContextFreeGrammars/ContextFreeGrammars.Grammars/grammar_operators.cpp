#include "stdafx.h"
#include "grammar_operators.h"
using namespace std;

term operator ""_term(const char *literal, size_t length)
{
	string value = { literal, length };
	return term{ value };
}

term operator ""_terminal(const char *literal, size_t length)
{
	string value{ literal, length };
	return term{ value, term_type::terminal };
}

term operator ""_nonterminal(const char *literal, size_t length)
{
	string value{ literal, length };
	return term{ value, term_type::rule_reference };
}

concatenation operator*(const term &left, const term &right)
{
	concatenation result;
	result.add_operand(left);
	result.add_operand(right);
	return result;
}

concatenation operator*(const concatenation &left, const term &right)
{
	concatenation result = left;
	result.add_operand(right);
	return result;
}

production operator|(const concatenation &left, const concatenation &right)
{
	production result;
	result.add_operand(left);
	result.add_operand(right);
	return result;
}

production operator|(const production &left, const concatenation &right)
{
	production result = left;
	result.add_operand(right);
	return result;
}

production operator|(const term &left, const concatenation & right)
{
	production result;
	result.add_operand(left);
	result.add_operand(right);
	return result;
}
