#include "stdafx.h"
#include "union_expression.h"

union_expression::~union_expression()
{
	delete left;
	delete right;
}
