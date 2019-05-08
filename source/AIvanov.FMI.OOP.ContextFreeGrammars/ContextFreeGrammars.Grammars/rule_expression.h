#pragma once
#include "stdafx.h"
#include "production_expression.h"
#include <string>
class rule_expression : public production_expression
{
	std::string ruleName;

public:
	rule_expression(std::string ruleName) :
		ruleName(ruleName) {}

	rule_expression(const rule_expression &toCopy) :
		ruleName(toCopy.ruleName) {}

	std::string rule_name()const;

	virtual production_expression *clone() const override;
};

