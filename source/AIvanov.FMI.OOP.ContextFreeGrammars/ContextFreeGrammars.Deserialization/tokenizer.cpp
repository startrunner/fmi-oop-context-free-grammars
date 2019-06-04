#include "stdafx.h"
#include "tokenizer.h"
#include <sstream>
using namespace std;

bool tokenizer::try_get_next_named(token *outToken)
{
	if (!try_cache(1))return false;

	int length = 0;
	token_type type;

	bool success = try_peek_valid(type, length) || try_peek_invalid(type, length);

	if (!success)return false;

	string value{ token_at.begin(), token_at.begin() + length };
	for (int i = 0; i < length; i++)token_at.pop_front();

	int tokenRow = this->row;
	int tokenColumn = this->column;
	advance_row_column(value, this->row, this->column);

	if (outToken == nullptr)return true;

	int rowsAdded = this->row - tokenRow;
	document_position position{ tokenRow, tokenColumn, length, rowsAdded };
	*outToken = token{ type, position, value };
	return true;
}

void tokenizer::refeed(std::istream *stream)
{
	row = column = 1;
	this->stream = stream;
	token_at.clear();
}

bool tokenizer::try_peek_invalid(token_type &type, int &length)
{
	token_type validType = {};
	int validLength = 0;

	length = 0;
	while (try_cache(length + 1) && !try_peek_valid(length, validType, validLength))length++;

	if (length == 0)return false;

	type = token_type::invalid_sequence;
	return true;
}

bool tokenizer::try_peek_valid(int from, token_type &type, int &length)
{
	return
		try_peek_terminal(from, type, length) ||
		try_peek_identifier(from, type, length) ||
		try_peek_whitespace(from, type, length);
}

bool tokenizer::try_peek_whitespace(int from, token_type &type, int &length)
{
	length = 0;
	while (try_cache(from + length + 1) && isspace(token_at[from + length]))length++;

	if (length == 0)return false;

	type = token_type::whitespace;
	return true;
}

bool tokenizer::try_peek_identifier(int from, token_type &type, int &length)
{
	length = 0;

	if (try_cache(from + 1) && (isalpha(token_at[from]) || token_at[from] == '_'))length++;

	if (length == 0)return false;

	while (
		try_cache(from + length + 1) &&
		(isalnum(token_at[from + length]) || token_at[from + length] == '_')
		)
	{
		length++;
	}

	if (isupper(token_at[from]))type = token_type::non_terminal;
	else type = token_type::terminal;

	return true;
}

bool tokenizer::try_peek_terminal(int from, token_type &type, int &length)
{
	length = 0;

	if (!try_cache(from + 1))return false;

	switch (token_at[from])
	{
	case '.':
		length = 1;
		type = token_type::concatenation;
		return true;
	case '|':
		length = 1;
		type = token_type::choice;
		return true;
	case ';':
		length = 1;
		type = token_type::semicolon;
		return true;
	}


	if (!try_cache(from + 2))return false;

	if (token_at[from + 0] == '-' && token_at[from + 1] == '>')
	{
		length = 2;
		type = token_type::arrow;
	}

	return length != 0;
}

void tokenizer::advance_row_column(const std::string &text, int &row, int &column)
{
	for (char character : text)
	{
		if (character == '\n')
		{
			row++;
			column = 1;
			continue;
		}

		column++;
	}
}