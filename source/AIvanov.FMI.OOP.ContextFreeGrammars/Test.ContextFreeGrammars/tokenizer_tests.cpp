#include "pch.h"
#include <string>
#include <sstream>
#include "../ContextFreeGrammars.Deserialization/tokenizer.h"
using namespace std;


const string GRAMMAR_TEXT =
"^^Expr -> Term | Expr.AddOp.Term | AddOp.Term;						\n"
"Term->Factor | Term.MulOp.Factor;									\n"
"Factor->Primary | Factor.pw.Primary;								\n"
"Primary->number | variable | opn.Expr.cls;							\n"
"AddOp->add | sub;													\n"
"MulOp->mul | div;";

vector<token_type> EXPECTED_MEANINGFUL_TOKENS{
	token_type::invalid_sequence,	//	^^
	token_type::non_terminal,		//	Expr
	token_type::arrow,				//	->
	token_type::non_terminal,		//	Term
	token_type::choice,				//	|
	token_type::non_terminal,		//	Expr
	token_type::concatenation,		//	.
	token_type::non_terminal,		//	AddOp
	token_type::concatenation,		//	.
	token_type::non_terminal,		//	Term
	token_type::choice,				//	|
	token_type::non_terminal,		//	AddOp
	token_type::concatenation,		//	.
	token_type::non_terminal,		//	Term
	token_type::semicolon,			//	;

	token_type::non_terminal,		//	Term
	token_type::arrow,				//	->
	token_type::non_terminal,		//	Factor
	token_type::choice,				//	|
	token_type::non_terminal,		//	Term
	token_type::concatenation,		//	.
	token_type::non_terminal,		//	MulOp
	token_type::concatenation,		//	.
	token_type::non_terminal,		//	Factor
	token_type::semicolon,			//	;

	token_type::non_terminal,		//	Factor
	token_type::arrow,				//	->
	token_type::non_terminal,		//	Primary
	token_type::choice,				//	|
	token_type::non_terminal,		//	Factor
	token_type::concatenation,		//	.
	token_type::terminal,			//	pw
	token_type::concatenation,		//	.
	token_type::non_terminal,		//	Primary
	token_type::semicolon,			//	;

	token_type::non_terminal,		//	Primary
	token_type::arrow,				//	->
	token_type::terminal,			//	number
	token_type::choice,				//	|
	token_type::terminal,			//	variable
	token_type::choice,				//	|
	token_type::terminal,			//	opn
	token_type::concatenation,		//	.
	token_type::non_terminal,		//	Expr
	token_type::concatenation,		//	.
	token_type::terminal,			//	cls
	token_type::semicolon,			//	;

	token_type::non_terminal,		//	AddOp	
	token_type::arrow,				//	->
	token_type::terminal,			//	add
	token_type::choice,				//	|
	token_type::terminal,			//	sub
	token_type::semicolon,			//	;

	token_type::non_terminal,		//	MulOp	
	token_type::arrow,				//	->
	token_type::terminal,			//	mul
	token_type::choice,				//	|
	token_type::terminal,			//	div
	token_type::semicolon			//	;
};

TEST(RestoreText, Tokenization)
{
	vector<token> tokens;

	istringstream stream{ GRAMMAR_TEXT };
	tokenizer tokenizer{ &stream };

	for (token t; tokenizer.try_get_next_named(&t);)tokens.push_back(t);

	ostringstream restored;
	for (token &t : tokens)
	{
		restored<<t.value();
	}

	string restoredStr = restored.str();

	ASSERT_EQ(GRAMMAR_TEXT, restoredStr);
}

TEST(CorrectMeaningfulTokenTypes, Tokenization)
{
	vector<token> meaningfulTokens;

	istringstream stream{ GRAMMAR_TEXT };
	tokenizer tokenizer{ &stream };

	for (token t; tokenizer.try_get_next_named(&t);)
	{
		if (t.type() == token_type::whitespace)continue;

		meaningfulTokens.push_back(t);
	}

	for (int i = 0; i < EXPECTED_MEANINGFUL_TOKENS.size(); i++)
	{
		token_type expected = EXPECTED_MEANINGFUL_TOKENS[i], given = meaningfulTokens[i].type();

		ASSERT_EQ(expected, given);
	}
}