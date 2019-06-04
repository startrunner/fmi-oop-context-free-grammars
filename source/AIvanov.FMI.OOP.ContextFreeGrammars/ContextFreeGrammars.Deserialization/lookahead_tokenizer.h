#pragma once
#include "stdafx.h"
#include "tokenizer.h"
#include <iosfwd>
#include <functional>
#include <climits>

class lookahead_tokenizer
{
public:
	using filter_predicate = typename std::function<bool(const token &token)>;

private:
	document_position lastPoppedPosition;
	tokenizer tokenizer;
	std::deque<token> cachedTokens;
	filter_predicate filter;

public:

	lookahead_tokenizer(std::istream *stream) :
		lookahead_tokenizer(stream, default_filter) {}

	lookahead_tokenizer(std::istream *stream, filter_predicate filter) :
		tokenizer(stream), filter(filter) {}

	void refeed(std::istream *stream);

	bool try_cache(int tokenCount);

	const token& token_at(int index);

	int cache_all();

	void pop(int count);
	token pop();

private:
	static bool default_filter(const token &token);
};