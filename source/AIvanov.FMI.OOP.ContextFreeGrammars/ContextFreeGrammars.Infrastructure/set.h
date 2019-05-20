#pragma once
#include <vector>
#include <string>
#include <initializer_list>
#include "entry_list.h"

template<typename T> class set
{
public:
	using iterator = typename entry_list<T>::iterator;
	using const_iterator = typename entry_list<T>::const_iterator;

private:
	entry_list<T> entries;

public:

	set() {}

	set(const set &toCopy) :entries(toCopy.entries) {}

	set(const std::initializer_list<T> &entries)
	{
		for (const T &item : entries)insert(item);
	}

	set& operator=(const set &toAssign)
	{
		if (this == &toAssign)return *this;

		entries = toAssign.entries;
		return *this;
	}

	void insert(const T& item)
	{
		if (count(item) != 0)return;
		entries.add(item);
	}

	size_t size()const { return entries.size(); }

	size_t count(const T& item)const
	{
		for (const T& candidate : entries)
		{
			if (candidate == item)return 1;
		}
		return 0;
	}

	bool empty()const { return entries.empty(); }

	iterator begin() { return entries.begin(); }
	const_iterator begin()const { return entries.begin(); }

	iterator end() { return entries.end(); }
	const_iterator end()const { return entries.end(); }

	iterator find(const T &value)
	{
		iterator result = begin();
		while (result != end() && *result != value)result++;
		return result;
	}

	const_iterator find(const T &value)const
	{
		iterator result = begin();
		while (result != end() && *result != value)result++;
		return result;
	}
};