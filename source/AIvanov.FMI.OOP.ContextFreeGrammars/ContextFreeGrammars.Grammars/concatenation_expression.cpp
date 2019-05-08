#include "stdafx.h"
#include "concatenation_expression.h"

concatenation_expression::~concatenation_expression()
{
	delete left;
	delete right;
}
