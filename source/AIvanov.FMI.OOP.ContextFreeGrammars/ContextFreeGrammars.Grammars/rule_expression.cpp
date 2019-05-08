#include "stdafx.h"
#include "rule_expression.h"

std::string rule_expression::rule_name() const
{
	return ruleName;
}

production_expression * rule_expression::clone() const
{
	return new rule_expression(*this);
}
