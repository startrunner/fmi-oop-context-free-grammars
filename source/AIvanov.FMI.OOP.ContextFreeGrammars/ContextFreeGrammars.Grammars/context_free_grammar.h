#pragma once
#include "stdafx.h"
#include "production_expression.h"

class context_free_grammar
{
	std::string rootRule;
	std::map<std::string, production_expression*> productionRules;
public:
	context_free_grammar();
	context_free_grammar(const context_free_grammar& toCopy);

	~context_free_grammar();
};