#pragma once
#include "stdafx.h"
#include "document_position.h"

enum class token_type
{
	non_terminal,
	terminal,
	concatenation,
	choice,
	arrow,
	whitespace,
	semicolon,

	invalid_sequence
};

class token
{
	struct {
		;
		token_type type;
		std::string value;
		document_position position;
	}my;

public:
	token() :
		token(token_type::arrow, document_position(1, 1, 2), "->") {}

	token(token_type type, document_position position, const std::string &value) :
		my({ type, value, position }) {}

	token_type type()const { return my.type; }
	void set_type(token_type type) { my.type = type; }

	const std::string &value()const { return my.value; }
	void set_value(const std::string &value) { my.value = value; }

	document_position position()const { return my.position; }
	void set_position(document_position position) { my.position = position; }
};