#include "stdafx.h"
#include "terminal_expression.h"

std::string terminal_expression::value() const
{
	return terminalValue;
}

production_expression *terminal_expression::clone() const
{
	return new terminal_expression(*this);
}
