#pragma once
#include <string>
#include "text_position.h"
#include "token_type.h"
class token
{
	std::string text;
	token_type type;
	text_position position;

public:

	const std::string& get_text()const { return text; }
	const token_type get_type()const { return type; }
	text_position get_position()const { return position; }

	token(token_type type, text_position position, const std::string &text) :
		text(text), type(type), position(position) {}

	token();
	~token();
};

