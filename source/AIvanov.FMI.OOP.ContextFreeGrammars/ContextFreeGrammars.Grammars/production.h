#pragma once
#include "concatenation.h"

class production :
	public variable_arity_operator<concatenation>
{
public:
	production() {}

	production(const char *toWrap) :
		production(std::string{ toWrap }) {}

	production(const std::string &toWrap) :
		production(term{ toWrap }) {}

	production(const term &toWrap) :
		production(concatenation{ toWrap }) {}

	production(const concatenation &toWrap);


	production(const production &toCopy) :
		variable_arity_operator<concatenation>(toCopy) {}

	void sort_operands();

	production& operator= (const production &toAssign);
};

