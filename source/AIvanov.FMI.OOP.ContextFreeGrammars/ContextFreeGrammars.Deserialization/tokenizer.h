#pragma once

#include <istream>
#include <deque>
#include "token.h"

class tokenizer
{
	std::istream *stream = nullptr;
	std::deque<char> token_at = {};
	int row = 1, column = 1;

public:
	tokenizer(std::istream *stream) :
		stream(stream) {}
	bool try_get_next_named(token *outToken = nullptr);
	void refeed(std::istream *stream);

private:

	bool try_peek_invalid(token_type &type, int &length);
	bool try_peek_valid(token_type &type, int &length) { return try_peek_valid(0, type, length); }
	bool try_peek_valid(int from, token_type &type, int &length);
	bool try_peek_whitespace(int from, token_type &type, int &length);
	bool try_peek_identifier(int from, token_type &type, int &length);
	bool try_peek_terminal(int from, token_type &type, int &length);

	bool try_cache(int lookaheadCount)
	{
		decltype(stream->get()) character;
		while (token_at.size() < lookaheadCount &&stream->good() && (character = stream->get()) != EOF)
		{
			token_at.push_back(character);
		}
		return token_at.size() >= lookaheadCount;
	}

	static void advance_row_column(const std::string &text, int &row, int &column);
};