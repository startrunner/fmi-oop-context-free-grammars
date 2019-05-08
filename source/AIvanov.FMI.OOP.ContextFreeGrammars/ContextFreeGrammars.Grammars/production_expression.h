#pragma once
#include <typeinfo>
#include <exception>
#include "stdafx.h"

class production_expression
{
public:
	virtual ~production_expression() {}
	virtual production_expression* clone()const = 0;
};
