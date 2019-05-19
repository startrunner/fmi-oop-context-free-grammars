#pragma once
#include "stdafx.h"
#include "term.h"
#include "variable_arity_operator.h"

class concatenation :
	public variable_arity_operator<term>
{
public:
	concatenation() {}

	concatenation(const char *toWrap) :
		concatenation(std::string{ toWrap }) {}

	concatenation(const std::string &toWrap) :
		concatenation(term{ toWrap }) {}

	concatenation(const term &toWrap);

	concatenation(const concatenation &toCopy) :
		variable_arity_operator<term>(toCopy) {}

	concatenation& operator= (const concatenation &toAssign);
};

