#pragma once
#include "production.h"
#include "cyk_parse_tree.h"

class grammar
{
	std::string startSymbol;
	map<std::string, production> productionRules;
public:
	grammar(
		const std::string &startSymbol,
		const map<std::string, production> &productionRules
	) :
		startSymbol(startSymbol),
		productionRules(productionRules) {}

	grammar() :
		grammar("_", {}) {}

	grammar(const grammar &toCopy) :
		startSymbol(toCopy.startSymbol), productionRules(toCopy.productionRules) {}

	grammar& operator=(const grammar &toAssign);

	bool operator== (const grammar &other)const;

	bool try_chomskify();
	std::string wrap_start_symbol();
	std::string wrap_start_symbol(const std::string &suggestedName);
	void eliminate_nonsolitary_terminals();
	void eliminate_long_concatenations();
	void inline_nullable_rules();
	void inline_unit_rules();
	void remove_unreferenced_rules();
	void sort_production_operands();

	bool is_chomskified()const;

	int rule_count()const;

	bool contains_rule(const std::string &ruleName)const;
	bool symbols_exist(const production &rule)const;

	std::vector<std::string> get_rule_names()const;

	const std::string& start_symbol()const;
	void set_start_symbol(const std::string &startSymbol);

	void add_rule(const std::string &ruleName, const production &expression);

	const production& get_rule(const std::string &ruleName)const;
	production& get_rule(const std::string &ruleName);

	void remove_rule(const std::string &ruleName);

	grammar union_with(const grammar &other)const;
	grammar concatenation_with(const grammar &other)const;
	grammar kleene_star()const;

	bool is_empty()const;

private:
	bool can_reach_terminal(const production &from, set<std::string> &walkedRules)const;
	bool can_reach_terminal(const concatenation &from, set<std::string> &walkedRules)const;

	void eliminate_nonsolitary_terminals(const std::string &ruleName, concatenation &rule)
	{
		if (rule.operand_count() == 0)
		{
			;
		}

		if (rule.operand_count() == 1)return;

		for (int i = 0; i < rule.operand_count(); i++)
		{
			if (!rule.operand(i).is_terminal())continue;

			std::string tval = rule.operand(i).value();

			term terminalRule =
				create_terminal_rule(ruleName, rule.operand(i).value());

			rule.replace_operand(i, terminalRule);
		}
	}

	void eliminate_long_concatenations(const std::string &ruleName, concatenation &rule);
	concatenation create_binary_concatenation(const std::string &ruleName, std::vector<term> operands);
	term create_concatenation_rule(const std::string &parentRuleName, const term &left, const term &right);

	static std::vector<concatenation> substitute_directly_nullable_rules(const concatenation &replaceIn, const set<std::string> &directlyNullables);
	void inline_directly_nullable_rules(const set<std::string> &toReplace);
	set<std::string> get_directly_nullable_rules(const set<std::string> &toIgnore)const;
	static bool is_directly_nullable(const production& rule);
	static bool is_directly_nullable(const concatenation& rule);
	void make_non_directly_nullable(const std::string &ruleName);
	static std::vector<int> get_directly_nullable_operand_indices(
		const concatenation &concatenation,
		const set<std::string> &directlyNullables
	);

	static void remove_repeating_concatenations(production &rule);

	set<std::string> get_referenced_rules()const;

	term create_terminal_rule(
		const std::string &suggestedName,
		const std::string &terminalValue
	);

	std::string mangle_name(const std::string &suggestedName, int startFrom = 1);

public:
	bool try_match(const std::string &word, cyk_parse_tree *outParseTree = nullptr)const;
	bool try_match(const std::vector<std::string> &word, cyk_parse_tree *outParseTree = nullptr)const;
private:

	struct binary_concatenation_rule
	{
		std::string ruleName, leftOperand, rightOperand;
		binary_concatenation_rule(const std::string &ruleName, const std::string &left, const std::string &right) :
			ruleName(ruleName), leftOperand(left), rightOperand(right) {}
	};

	std::vector<binary_concatenation_rule> get_binary_concatenation_rules()const;

	std::vector<std::vector<map<std::string, cyk_parse_tree_node>>> initialize_cyk_table(const std::vector<std::string> &word)const;
};

