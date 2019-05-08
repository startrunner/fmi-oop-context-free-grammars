#pragma once
#include "stdafx.h"
#include <string>
#include "production_expression.h"
class terminal_expression : public production_expression
{
	std::string terminalValue;
public:
	terminal_expression() :terminal_expression("") {}

	terminal_expression(std::string value)
		: terminalValue(value) {}

	terminal_expression(const terminal_expression &toCopy) :
		terminalValue(toCopy.terminalValue) {}

	std::string value()const;

	// Inherited via production_expression
	virtual production_expression * clone() const override;
};

