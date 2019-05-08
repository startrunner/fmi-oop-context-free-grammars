#pragma once
#include <variant>

template<typename TItem, typename TVariant>
inline bool is(const TVariant variant, TItem *outItem = nullptr)
{
	if (std::holds_alternative<TItem>(variant))
	{
		if(outItem!=nullptr)(*outItem) = std::get<TItem>(variant);
		return true;
	}
	return false;
}