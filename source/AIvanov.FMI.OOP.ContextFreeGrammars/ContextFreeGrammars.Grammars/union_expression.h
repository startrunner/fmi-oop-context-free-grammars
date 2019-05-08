#pragma once
#include "stdafx.h"
#include "production_expression.h"
class union_expression
{
	production_expression *left, *right;
public:
	union_expression(const union_expression &toCopy) :
		left(toCopy.left->clone()),
		right(toCopy.right->clone()) {}

	union_expression(const production_expression &left, const production_expression &right) :
		left(left.clone()),
		right(right.clone()) {}

	virtual ~union_expression();
};

