#include "stdafx.h"
#include <exception>
#include "lookahead_tokenizer.h"
using namespace std;

lookahead_tokenizer & lookahead_tokenizer::refeed(std::shared_ptr<std::istream> characters)
{
	cachedTokens.clear();
	tokenizer.refeed(characters);
	return *this;
}

bool lookahead_tokenizer::can_get() const
{
	return !cachedTokens.empty() || tokenizer.can_get();
}

bool lookahead_tokenizer::try_get(token *outToken)
{
	if (!cachedTokens.empty())
	{
		*outToken = cachedTokens.front();
		cachedTokens.pop_front();
		return true;
	}

	return tokenizer.try_get(outToken);
}

token lookahead_tokenizer::get()
{
	token token;
	if (!try_get(&token))
	{
		throw runtime_error("Can't get, EOF reached.");
	}

	return token;
}

bool lookahead_tokenizer::try_cache(int tokenCount)
{
	while (cachedTokens.size() < tokenCount && tokenizer.can_get())
	{
		cachedTokens.push_back(tokenizer.get());
	}

	return cachedTokens.size() >= tokenCount;
}

bool lookahead_tokenizer::try_peek(int index, token *outToken)
{
	if (!try_cache(index + 1))return false;

	*outToken = cachedTokens[index];
	return true;
}

token lookahead_tokenizer::peek(int index)
{
	token token;
	if (!try_peek(index, &token))
	{
		throw runtime_error("Can't get from token stream after EOF.");
	}

	return token;
}

lookahead_tokenizer::~lookahead_tokenizer()
{
}
