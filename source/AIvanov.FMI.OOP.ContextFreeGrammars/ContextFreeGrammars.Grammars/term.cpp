#include "stdafx.h"
#include "term.h"
#include <exception>
using namespace std;

const std::string& term::value() const
{
	return my.value;
}

term& term::set_value(const std::string &value)
{
	validate_value_or_throw(value);
	my.value = value;
	return *this;
}

const term_type term::type() const
{
	return my.type;
}

term& term::set_type(term_type type)
{
	my.type = type;
	return *this;
}

bool term::is_empty_word() const
{
	return my.value == "_";
}

term& term::make_empty_word()
{
	my.value = "_";
	my.type = term_type::terminal;
	return *this;
}

bool term::is_terminal() const
{
	return my.type == term_type::terminal;
}

term_type term::determine_term_type(const string &value)
{
	validate_value_or_throw(value);

	return
		(value == "_" || islower(value.front())) ?
		term_type::terminal :
		term_type::rule_reference;
}

bool term::operator<(const term & other) const
{
	if (is_empty_word() && !other.is_empty_word())return false;
	return type() < other.type() || (type() == other.type() && value() < other.value());
}

bool term::operator==(const term & other) const
{
	return type() == other.type() && value() == other.value();
}

bool term::validate_value(const string &value)
{
	if (value.empty())return false;
	if (value == "_")return true;

	bool valid = isalpha(value.front()) || value.front() == '_';
	for (int i = 1; valid && i < value.size(); i++)
	{
		valid &= isalnum(value[i]) || value[i] == '_';
	}
	return valid;
}
