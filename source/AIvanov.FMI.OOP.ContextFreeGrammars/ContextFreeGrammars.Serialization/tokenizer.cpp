#include <exception>
#include <string>
#include <cctype>
#include <istream>
#include "stdafx.h"
#include "tokenizer.h"
#include "try_parse_token.h"
using namespace std;

const map<char, token_type> SingleCharTokenTypes = {
	{'(', token_type::left_round},
	{')', token_type::right_round},
	{':', token_type::colon},
	{';', token_type::semicolon}
};

string to_string(char character)
{
	return string{ 1, character };
}

bool is_identifier_character(char c)
{
	return isalnum(c) || c == '_';
}

tokenizer::tokenizer(std::shared_ptr<std::istream> characters)
{
	this->characters = characters;
}

tokenizer &tokenizer::refeed(std::shared_ptr<std::istream> characters)
{
	this->characters = characters;
	this->lastPosition = {};
	return *this;
}

bool tokenizer::can_get() const { return characters->good(); }

bool tokenizer::try_get(token *outToken)
{
	istream &characters = *this->characters;

	bool success = false
		|| try_parse(characters, lastPosition, isalpha, is_identifier_character, token_type::identifier, outToken)
		|| try_parse(characters, lastPosition, isspace, isspace, token_type::whitespace, outToken)
		|| try_parse(characters, lastPosition, '\'', token_type::literal, outToken)
		|| try_parse(characters, lastPosition, SingleCharTokenTypes, outToken)
		|| try_parse(characters, lastPosition, outToken);


	if(!success) return false;

	lastPosition = outToken->get_position();
	return true;
}

token tokenizer::get()
{
	token token;
	if (!try_get(&token))throw runtime_error("Can't get empty token.");
	return token;
}

tokenizer::~tokenizer() {}
