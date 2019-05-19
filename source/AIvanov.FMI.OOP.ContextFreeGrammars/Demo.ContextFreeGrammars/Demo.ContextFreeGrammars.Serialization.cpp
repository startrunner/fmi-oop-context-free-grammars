#include "pch.h"
#include "../ContextFreeGrammars.Grammars/grammar.h"
#include "../ContextFreeGrammars.Grammars/grammar_operators.h"
#include "../ContextFreeGrammars.Serialization/grammar_serialization.h"
#include <iostream>
#include <sstream>
using namespace std;

/*grammar g = {
	"Stamat",
	{
		{"Stamat", "kiro"_term * "Pesho" | "Blagoi"_term * "Haho" | "Nulata"_term * "Pesho" | "Nulata"},
		{"Pesho", "Haho"_term | "Haho"_term*"Pesho" | "_"},
		{"Haho", "Blagoi"_term | "Haho" | "Stamat" | "Georgi"_term * "Georgi" | "stamat"_terminal * "Georgi" * "Dulgiq"},
		{"Dulgiq", "Stamat"_term * "Pesho"*"Haho" * "gosho" | "ivan"_term * "Stamat" * "petkan" | "GoshoUnita"},
		{"GoshoUnita", "Gosho"_term},
		{"Gosho", "DrugiqGoshoUnit"},
		{"DrugiqGoshoUnit", "petko"},
		{"Nulata", "nulata"_term | "_"},
		{"Blagoi", "blago_we"}
	}
};*/

grammar g = {
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

int main()
{
	cout << "grammar" << endl;
	cout << g << endl;

	cout << "start" << endl;
	g.wrap_start_symbol();
	cout << g << endl;

	cout << "term" << endl;
	g.eliminate_nonsolitary_terminals();
	cout << g << endl;

	cout << "bin" << endl;
	g.eliminate_long_concatenations();
	cout << g << endl;

	cout << "del" << endl;
	g.inline_nullable_rules();
	cout << g << endl;

	cout << "unit" << endl;
	g.inline_unit_rules();
	cout << g << endl;

	cout << "sort" << endl;
	g.sort_production_operands();
	cout << g << endl;

	cout << "remove unref. " << endl;
	g.remove_unreferenced_rules();
	cout << g << endl;

	cout << "chomskified: " << g.is_chomskified() << endl;

	vector<string> cykWords = {
		"number add number",
		"number mul number add opn number div number cls mul number"
	};

	for (string &word : cykWords)
	{
		bool result;
		cyk_parse_tree parseTree;

		cout << "CYK(" << word << "): " << (result = g.try_match(word, &parseTree)) << endl;

		if (result)
		{
			cout << parseTree << endl;
		}
	}

	stringstream serializationStream;
	serializationStream << g;

	grammar deserialized;
	if (try_deserialize_grammar(serializationStream, &deserialized))
	{
		cout << "deserialization succeeded" << endl;
		cout << "equality: " << (g == deserialized) << endl;
		cout << deserialized << endl;
	}
	else cout << "deserialization failed" << endl;
}