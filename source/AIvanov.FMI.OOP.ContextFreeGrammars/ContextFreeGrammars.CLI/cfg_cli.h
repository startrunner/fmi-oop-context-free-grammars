#pragma once
#include "pch.h"
#include "../ContextFreeGrammars.Grammars/grammar.h"

struct command
{
	std::string name;
	std::vector<std::string> arguments;

	command() {}
	command(const std::string &name, const std::vector<std::string> &arguments) :
		name(name), arguments(arguments) {}
};

class cfg_cli
{
	map<int, grammar> grammars;

public:
	void run();

	command read_command();

	void open(const std::vector<std::string> &arguments);
	void list()const;
	void print(const std::vector<std::string> &arguments)const;
	void save(const std::vector<std::string> &arguments);
	void grammar_union(const std::vector<std::string> &arguments);
	void grammar_concatenation(const std::vector<std::string> &arguments);
	void grammar_kleene_star(const std::vector<std::string> &arguments);
	void grammar_check_chomsky(const std::vector<std::string> &arguments)const;
	void grammar_check_empty(const std::vector<std::string> &arguments)const;
	void grammar_chomskify(const std::vector<std::string> &arguments);
	void add_rule(const std::vector<std::string> &arguments);
	void remove_rule(const std::vector<std::string> &arguments);
	void cyk(const std::vector<std::string> &arguments)const;


	bool try_get_grammar_from_id(const std::vector<std::string> &arguments, grammar const **outGrammar)const;
	bool try_get_two_grammars_from_ids(const std::vector<std::string> &arguments, grammar const **outLeft, grammar const **outRight)const;

	bool show_binary_dialog(const std::string &message, const std::string &yes, const std::string &no)const;

	cfg_cli();
	~cfg_cli();
};

