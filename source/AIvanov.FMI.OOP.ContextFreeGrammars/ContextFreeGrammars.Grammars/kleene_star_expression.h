#pragma once
#include "stdafx.h"
#include "production_expression.h"

class kleene_star_expression : public production_expression
{
	production_expression *expression;
public:
	kleene_star_expression(const production_expression &expression) :
		expression(expression.clone()) {}

	kleene_star_expression(const kleene_star_expression &toCopy) :
		expression(toCopy.expression->clone()) {}

	virtual ~kleene_star_expression();
};

