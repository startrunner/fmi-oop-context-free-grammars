#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <initializer_list>
#include "entry_list.h"


template<typename TKey, typename TValue> class map
{
	using iterator = typename entry_list<std::pair<TKey, TValue>>::iterator;
	using const_iterator = typename entry_list<std::pair<TKey, TValue>>::const_iterator;


private:
	entry_list<std::pair<TKey, TValue>> entries;

public:

	map() { }

	map(const map &toCopy) { entries = toCopy.entries; }

	map(const std::initializer_list<std::pair<TKey, TValue>> &items)
	{
		for (const auto &entry : items)insert(entry);
	}

	bool empty()const { return entries.empty(); }
	size_t size()const { return entries.size(); }

	iterator begin() { return entries.begin(); }
	const_iterator begin()const { return entries.begin(); }

	iterator end() { return entries.end(); }
	const_iterator end()const { return entries.end(); }

	iterator find(const TKey &key)
	{
		iterator result = begin();
		while (result != end() && (*result).first != key)result++;
		return result;
	}

	const_iterator find(const TKey &key)const
	{
		iterator result = begin();
		while (result != end() && (*result).first != key)result++;
		return result;
	}

	TValue& operator[](const TKey &key)
	{
		if (count(key) == 0)entries.add({ key, {} });
		return at(key);
	}

	TValue& at(const TKey &key)
	{
		return (*find(key)).second;
	}

	const TValue& at(const TKey &key)const
	{
		if (count(key) == 0)throw std::out_of_range("nada");
		return (*find(key)).second;
	}

	bool insert(const std::pair<TKey, TValue> &entry)
	{
		return insert(entry.first, entry.second);
	}

	bool emplace(const TKey &key, const TValue &value) { return insert(key, value); }

	bool insert(const TKey &key, const TValue &value)
	{
		if (count(key) != 0)return false;
		entries.add({ key, value });
	}

	size_t count(const TKey &key)const
	{
		for (const auto &entry : entries)
		{
			if (entry.first == key)return 1;
		}

		return 0;
	}

	size_t erase(const TKey &key)
	{
		if (count(key) == 0)return 0;

		const TValue &value = at(key);
		entries.remove({key, value});

		return 1;
	}

	bool operator == (const map &other)const
	{
		if (this == &other)return true;
		if (size() != other.size())return false;

		std::vector<std::pair<TKey, TValue>>left, right;

		for (const auto &leftItem : this->entries)left.push_back(leftItem);
		for (const auto &rightItem : other.entries)right.push_back(rightItem);

		std::sort(left.begin(), left.end());
		std::sort(right.begin(), right.end());

		return left == right;
	}
};
