#include "stdafx.h"
#include "kleene_star_expression.h"

kleene_star_expression::~kleene_star_expression()
{
	delete expression;
}
