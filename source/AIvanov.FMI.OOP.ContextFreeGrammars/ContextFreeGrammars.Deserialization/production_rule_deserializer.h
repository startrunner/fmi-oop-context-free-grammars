#pragma once
#include "stdafx.h"
#include "lookahead_tokenizer.h"
#include "../ContextFreeGrammars.Grammars/production.h"

class production_rule_deserializer
{
	lookahead_tokenizer tokenizer;

public:
	production_rule_deserializer(std::istream *stream) :
		tokenizer(stream, filter_token_no_whitespace) {}

	void refeed(std::istream *stream);

	bool try_get_next_named(std::string *outName = nullptr, production *outRule = nullptr);
	bool try_get_next(production *outRule = nullptr);

private:

	production parse_production(int firstIndex, int lastIndex);
	concatenation parse_concatenation(int firstIndex, int lastIndex);
	std::vector<int> find_indices(int firstIndex, int lastIndex, token_type type);

	bool validate_expression(int firstIndex, int lastIndex);

	bool try_peek_next_named(
		std::string *outName = nullptr,
		int *outExpressionStartIndex = nullptr,
		int *outExpressionSpan = nullptr,
		int *outSpan = nullptr
	);

	int index_of_or_negative(token_type type);

	static bool filter_token_no_whitespace(const token &token);
};