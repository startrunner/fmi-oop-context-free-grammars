#include "stdafx.h"
#include "bitmaskutil.h"
using namespace std;

bool try_advance_bitmask(vector<bool> &bitset)
{
	bool allTrue = true;
	for (bool value : bitset)allTrue &= value;

	if (allTrue) return false;

	for (int i = bitset.size() - 1; i >= 0; i--)
	{
		if (!bitset[i])return bitset[i] = true;
		else bitset[i] = false;
	}

	throw runtime_error("failed to advance bitset.");
}

bool is_all_false(const std::vector<bool>& bitmask)
{
	bool allFalse = true;
	for (int i = 0; allFalse && i < bitmask.size(); i++)allFalse &= !bitmask[i];
	return allFalse;
}