#include "stdafx.h"
#include "concatenation.h"

concatenation::concatenation(const term & toWrap)
{
	add_operand(toWrap);
}

concatenation& concatenation::operator=(const concatenation &toAssign)
{
	if (this == &toAssign)return *this;
	variable_arity_operator<term>::operator=(toAssign);
	return *this;
}