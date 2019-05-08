#pragma once
#include "stdafx.h"
#include "production_expression.h"
class concatenation_expression
{
	production_expression *left, *right;
public:
	concatenation_expression(const production_expression &left, const production_expression &right) :
		left(left.clone()),
		right(right.clone()) {}

	concatenation_expression(const concatenation_expression &toCopy) :
		left(toCopy.left->clone()),
		right(toCopy.right->clone()) {}

	virtual ~concatenation_expression();
};

