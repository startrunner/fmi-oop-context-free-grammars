#pragma once
#include "stdafx.h"

enum class term_type : char
{
	terminal = 't',
	rule_reference = 'r'
};

class term
{
	struct
	{
		std::string value;
		term_type type;
	}my;

public:
	static const std::string EMPTY_WORD;

	term() :
		term(EMPTY_WORD) {}

	term(const char *value) :
		term(std::string{ value }) {}

	term(const std::string &value) :
		term(value, determine_term_type(value)) {}

	term(const std::string &value, term_type type) :
		my({ value, type })
	{
		validate_value_or_throw(value);
	}

	const std::string& value()const;
	term& set_value(const std::string &value);

	const term_type type()const;
	term& set_type(term_type type);

	bool is_empty_word()const;
	term& make_empty_word();

	bool is_terminal()const;

	static term_type determine_term_type(const std::string &value);

	bool operator < (const term &other)const;
	bool operator == (const term &other)const;

	static inline void validate_value_or_throw(const std::string &value)
	{
		if (!validate_value(value))
		{
			throw std::runtime_error("invalid term value '" + value + "'");
		}
	}

	static bool validate_value(const std::string &value);
};