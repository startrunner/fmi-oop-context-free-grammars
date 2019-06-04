#include "stdafx.h"
#include "lookahead_tokenizer.h"
#include <vector>
#include <exception>
using namespace std;

void lookahead_tokenizer::refeed(std::istream *stream)
{
	tokenizer.refeed(stream);
	cachedTokens.clear();
	lastPoppedPosition = {};
}

bool lookahead_tokenizer::try_cache(int tokenCount)
{
	if (tokenCount < 0)return false;

	for (token t; cachedTokens.size() < tokenCount && this->tokenizer.try_get_next_named(&t);)
	{
		if (!filter(t))continue;
		cachedTokens.push_back(t);
	}

	return cachedTokens.size() >= tokenCount;
}

const token &lookahead_tokenizer::token_at(int index)
{
	if (!try_cache(index + 1))
	{
		throw std::runtime_error(
			"Could not cache " + to_string(index + 1) + " "
			"tokens in order to access the one at index " + to_string(index) + "."
		);
	}

	return cachedTokens[index];
}

int lookahead_tokenizer::cache_all()
{
	for (int i = 1; try_cache(i); i++) {}
	return this->cachedTokens.size();
}

void lookahead_tokenizer::pop(int count)
{
	if (count == 0)return;

	if (count < 0 && !try_cache(count))
	{
		throw runtime_error(
			"Could not pop '" + to_string(count) + "' tokens because only "
			"" + to_string(cachedTokens.size()) + " are available."
		);
	}

	token lastToken = cachedTokens[count - 1];
	lastPoppedPosition = lastToken.position();

	if (count == cachedTokens.size())cachedTokens.clear();
	else for (int i = 0; i < count; i++)cachedTokens.pop_front();
}

token lookahead_tokenizer::pop()
{
	if (!try_cache(1))
	{
		throw runtime_error(
			"Could not pop token because none are available."
		);
	}

	token token = cachedTokens.front();
	lastPoppedPosition = token.position();
	cachedTokens.pop_front();

	return token;
}

bool lookahead_tokenizer::default_filter(const token & token)
{
	return true;
}
