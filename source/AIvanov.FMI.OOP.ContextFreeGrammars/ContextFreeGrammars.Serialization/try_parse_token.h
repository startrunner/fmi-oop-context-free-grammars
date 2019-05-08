#pragma once
#include "stdafx.h"
#include <istream>
#include <functional>
#include <map>
#include <string>
#include "token.h"
bool try_parse(
	std::istream &characters,
	text_position lastPosition,
	std::function<bool(char)> firstCharPredicate,
	std::function<bool(char)> continuationPredicate,
	token_type tokenType,
	token *outToken
);

bool try_parse(
	std::istream &characters,
	text_position lastPosition,
	char quotationMarkCharacter,
	token_type tokenType,
	token *outToken
);

bool try_parse(
	std::istream &characters,
	text_position lastPosition,
	const std::map<char, token_type> &tokenTypesByCharacter,
	token *outToken
);

bool try_parse(std::istream &characters, text_position lastPosition, token *outToken);

bool try_parse(
	std::istream &characters,
	std::function<bool(char)> firstCharPedicate,
	std::function<bool(char)> continuationPredicate,
	std::string *outText
);

bool try_parse(
	std::istream &characters,
	char quotationMarkCharacter,
	std::string *outText
);