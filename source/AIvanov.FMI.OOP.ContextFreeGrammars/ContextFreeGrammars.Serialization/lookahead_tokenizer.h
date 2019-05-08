#pragma once
#include <memory>
#include <istream>
#include <deque>
#include "tokenizer.h"
class lookahead_tokenizer
{
	std::deque<token> cachedTokens = {};
	tokenizer tokenizer;

public:
	lookahead_tokenizer(std::shared_ptr<std::istream> characters)
		:tokenizer(characters) {}

	lookahead_tokenizer& refeed(std::shared_ptr<std::istream> characters);

	bool can_get() const;
	bool try_get(token *outToken);
	token get();

	bool try_cache(int tokenCount);
	bool try_peek(int index, token *outToken);
	token peek(int index);


	~lookahead_tokenizer();
};

