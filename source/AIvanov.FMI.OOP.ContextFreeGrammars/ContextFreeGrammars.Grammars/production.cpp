#include "stdafx.h"
#include "production.h"
using namespace std;

production::production(const concatenation & toWrap)
{
	add_operand(toWrap);
}

void production::sort_operands()
{
	vector<concatenation> operands;
	for (int i = 0; i < operand_count(); i++)operands.push_back(operand(i));

	sort(operands.begin(), operands.end());

	clear_operands();
	for (concatenation &operand : operands)add_operand(operand);
}

production& production::operator=(const production &toAssign)
{
	if (this == &toAssign)return *this;
	variable_arity_operator<concatenation>::operator=(toAssign);
	return *this;
}
