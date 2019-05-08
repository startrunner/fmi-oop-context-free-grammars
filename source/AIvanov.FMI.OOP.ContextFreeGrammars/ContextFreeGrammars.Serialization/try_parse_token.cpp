#include "stdafx.h"
#include <sstream>
#include "try_parse_token.h"
using namespace std;

bool try_parse(
	istream &characters,
	text_position lastPosition,
	function<bool(char)> firstCharPredicate,
	function<bool(char)> continuationPredicate,
	token_type tokenType,
	token *outToken
)
{
	string text;
	if (!try_parse(
		characters,
		firstCharPredicate,
		continuationPredicate,
		&text
	))
	{
		return false;
	}

	*outToken = token{ 
		tokenType, 
		lastPosition.get_next(text), 
		text 
	};
	return true;
}

bool try_parse(
	istream &characters,
	function<bool(char)> firstCharPedicate,
	function<bool(char)> continuationPredicate,
	string *outText
)
{
	if (!characters.good() || !firstCharPedicate(characters.peek()))
	{
		return false;
	}

	stringstream text;

	while (continuationPredicate(characters.peek()))
	{
		text << characters.get();
	}

	*outText = text.str();
	return true;
}

bool try_parse(
	istream &characters,
	text_position lastPosition,
	const map<char, token_type> &tokenTypesByCharacter,
	token *outToken
)
{
	if (!characters.good())return false;

	auto found = tokenTypesByCharacter.find(characters.peek());
	if (found == tokenTypesByCharacter.end())return false;

	string text = to_string(characters.get());

	*outToken = token{
		found->second,
		lastPosition.get_next(text),
		text
	};
	return true;
}

bool try_parse(
	istream &characters,
	text_position lastPosition,
	char quotationMarkCharacter,
	token_type tokenType,
	token *outToken
)
{
	string text;

	if (!try_parse(characters, quotationMarkCharacter, &text))
	{
		return false;
	}

	text_position position = lastPosition.get_next(text);

	if (text.size() < 2 || text.back() != quotationMarkCharacter)
	{
		*outToken = token{ token_type::unsupported, position, text };
		return true;
	}

	*outToken = token{ tokenType, position, text };
	return true;
}

bool try_parse(
	istream &characters,
	char quotationMarkCharacter,
	string *outText
)
{
	if (!characters.good() || characters.peek() != quotationMarkCharacter)
	{
		return false;
	}

	stringstream text;
	text << characters.get();

	while (characters.good() && characters.peek() != quotationMarkCharacter)
	{
		text << characters.get();
	}

	if (characters.good())
	{
		text << characters.get();
	}

	*outText = text.str();
	return true;
}

bool try_parse(istream &characters, text_position lastPosition, token *outToken)
{
	if (!characters.good())return false;

	string text = to_string(characters.get());

	*outToken = token{
		token_type::unsupported, 
		lastPosition.get_next(text),
		text
	};
}