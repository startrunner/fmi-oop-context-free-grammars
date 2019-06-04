#include "pch.h"
#include "../ContextFreeGrammars.Serialization/grammar_serialization.h"
#include "../ContextFreeGrammars.Deserialization/grammar_deserialization.h"
#include "../ContextFreeGrammars.Serialization/informal_grammar_serialization.h"
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

TEST(RestoredProperly, SerializeDeserialize)
{
	stringstream stream;
	serialize_grammar(stream, TEST_GRAMMAR);

	grammar deserialized;

	std::string serializedText = stream.str(); ;

	bool deserializationSuccess = try_deserialize_grammar(stream, &deserialized);

	if (deserializationSuccess)
	{
		ostringstream informallyReserializedStream;
		informallyReserializedStream << deserialized;
		string informallyReserialized = informallyReserializedStream.str();
	}

	ASSERT_TRUE(deserializationSuccess);
	ASSERT_TRUE(deserialized == TEST_GRAMMAR);
	ASSERT_EQ(deserialized, TEST_GRAMMAR);
}