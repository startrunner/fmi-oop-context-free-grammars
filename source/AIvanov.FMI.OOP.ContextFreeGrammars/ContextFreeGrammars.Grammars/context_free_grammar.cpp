#include "stdafx.h"
#include "context_free_grammar.h"



context_free_grammar::context_free_grammar()
{
}

context_free_grammar::context_free_grammar(const context_free_grammar & toCopy)
{
	this->rootRule = toCopy.rootRule;

	for (const auto &x : toCopy.productionRules)
	{
		this->productionRules.emplace(x.first, x.second->clone());
	}
}


context_free_grammar::~context_free_grammar()
{
	for (auto &x : this->productionRules)delete x.second;
}
