#pragma once
#include "stdafx.h"

template <typename T>
class variable_arity_operator
{
	std::vector<T> operands;

protected:
	variable_arity_operator() {}

	variable_arity_operator(const variable_arity_operator<T> &toClone) :
		operands(toClone.operands) {}

	variable_arity_operator& operator= (const variable_arity_operator &toAssign)
	{
		if (this == &toAssign)return *this;
		operands = toAssign.operands;
		return *this;
	}
public:

	bool has_no_operands()const { return operands.empty(); }

	int operand_count()const 
	{
		int value = operands.size();
		;
		return value;
	}

	T& operand(int index) { return operands[index]; }

	const T& operand(int index)const { return operands[index]; }

	void add_operand(const T& operand)
	{
		operands.push_back(operand);
	}

	template<typename TCollection> void add_operands(const TCollection &operands)
	{
		for (const T &operand : operands)add_operand(operand);
	}

	void replace_operand(int index, const T& operand)
	{
		if (index < 0 || index >= operand_count())
		{
			throw std::out_of_range("");
		}
		
		operands[index] = operand;
	}

	void remove_last_operand()
	{
		operands.pop_back();
	}

	void clear_operands()
	{
		operands.clear();
	}

	void remove_operands(const set<int> &indicesToRemove)
	{
		std::vector<T> keptOperands;
		for (int i = 0; i < operands.size(); i++)
		{
			if (indicesToRemove.count(i) != 0)continue;
			keptOperands.push_back(operands[i]);
		}

		operands = keptOperands;
	}

	bool operator < (const variable_arity_operator<T> &other)const
	{
		if (this == &other)return false;
		return operands < other.operands;
	}

	bool operator == (const variable_arity_operator<T> &other)const
	{
		if (this == &other)return true;
		return operands == other.operands;
	}

public:
	virtual ~variable_arity_operator() {}
};
