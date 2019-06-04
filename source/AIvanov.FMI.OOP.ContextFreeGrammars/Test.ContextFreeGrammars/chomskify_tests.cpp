#include "pch.h"
#include <vector>
#include <tuple>
#include "../ContextFreeGrammars.Grammars/grammar.h"
#include "../ContextFreeGrammars.Grammars/grammar_operators.h"
using namespace std;

const grammar TEST_GRAMMAR = {
	"Expr",
	{
		{"Expr", "Term"_term | "Expr"_term * "AddOp" * "Term" | "AddOp"_term*"Term"},
		{"Term", "Factor"_term | "Term"_term*"MulOp"*"Factor"},
		{"Factor", "Primary"_term | "Factor"_term*"pw"*"Primary"},
		{"Primary", "number"_terminal | "variable" | "opn"_terminal*"Expr"*"cls"},
		{"AddOp", "add"_terminal | "sub"},
		{"MulOp", "mul"_terminal | "div"}
	}
};

grammar get_chomskified(const grammar &grammar)
{
	struct grammar chomskified = grammar;
	chomskified.try_chomskify();
	return chomskified;
}

const grammar CHOMSKIFIED_TEST_GRAMMAR = get_chomskified(TEST_GRAMMAR);

TEST(ChomskifySucceeds, Chomsky) 
{
	EXPECT_TRUE(CHOMSKIFIED_TEST_GRAMMAR.is_chomskified());
}

TEST(CykSucceedsOnGoodWords, Chomsky)
{
	static const vector<string> goodWords = {
		"number add number",
		"number mul number add opn number div number cls mul number"
	};

	for (const std::string &word : goodWords)
	{
		cyk_parse_tree parseTree;
		ASSERT_TRUE(CHOMSKIFIED_TEST_GRAMMAR.try_match(word, &parseTree));
	}
}

