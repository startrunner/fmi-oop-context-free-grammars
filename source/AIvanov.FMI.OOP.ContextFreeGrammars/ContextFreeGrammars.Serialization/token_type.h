#pragma once

enum class token_type 
{
	identifier,
	literal,
	op_union,
	op_concatenation,

	left_round,
	right_round,

	colon,
	semicolon,

	whitespace,
	unsupported
};
