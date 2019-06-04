#include "pch.h"
#include "cfg_cli.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include "../ContextFreeGrammars.Grammars/grammar.h"
#include "../ContextFreeGrammars.Infrastructure/stringutil.h"
#include "../ContextFreeGrammars.Serialization/informal_grammar_serialization.h"
#include "../ContextFreeGrammars.Deserialization/grammar_deserialization.h"
#include "../ContextFreeGrammars.Serialization/grammar_serialization.h"
using namespace std;

void cfg_cli::run()
{
	for (bool running = true; running;)
	{
		command command = read_command();

		if (trim(command.name, ' ') == "") {}
		else if (command.name == "exit")running = false;
		else if (command.name == "open")open(command.arguments);
		else if (command.name == "list")list();
		else if (command.name == "print")print(command.arguments);
		else if (command.name == "save")save(command.arguments);
		else if (command.name == "union")grammar_union(command.arguments);
		else if (command.name == "concat")grammar_concatenation(command.arguments);
		else if (command.name == "chomsky")grammar_check_chomsky(command.arguments);
		else if (command.name == "chomskify")grammar_chomskify(command.arguments);
		else if (command.name == "iter")grammar_kleene_star(command.arguments);
		else if (command.name == "cyk")cyk(command.arguments);
		else if (command.name == "addrule")add_rule(command.arguments);
		else if (command.name == "removerule")remove_rule(command.arguments);
		else
		{
			cout << "invalid command '" << command.name << "'" << endl;
		}
	}
}

command cfg_cli::read_command()
{
	string line;
	getline(cin, line);
	vector<string> split = split_trim(line, ' ', ' ');

	string commandName = "";
	if (!split.empty())commandName = to_lower(split.front());

	vector<string> arguments;
	for (int i = 1; i < split.size(); i++)arguments.push_back(split[i]);

	return command{ commandName, arguments };
}

void cfg_cli::open(const std::vector<std::string> &arguments)
{
	if (arguments.empty())
	{
		cout << "Expected argument: <File Path>. Got none instead." << endl;
		return;
	}

	string path = concatenate(arguments);
	if (!filesystem::exists(path))
	{
		cout << "Error: file '" << path << "' does not exist." << endl;
		return;
	}

	grammar grammar;
	ifstream stream{ path };

	if (!try_deserialize_grammar(stream, &grammar))
	{
		cout << "could not deserialize grammar. make sure file is in correct format." << endl;
		return;
	}

	int id = grammars.size();
	grammars[id] = grammar;

	cout << "Gramar read successfilly.  ID: " << id << endl;
}

void cfg_cli::list()const
{
	cout << "Available grammars: ";
	if (!grammars.empty())cout << "0..." << grammars.size() - 1 << endl;
	else cout << "none" << endl;
}

void cfg_cli::print(const vector<string>& arguments) const
{
	grammar const *grammar;
	if (!try_get_grammar_from_id(arguments, &grammar))return;

	cout << (*grammar) << endl;
}

void cfg_cli::save(const std::vector<std::string>& arguments)
{
	if (arguments.size() < 2)
	{
		cout << "Expected two arguments: <Grammar ID> <File Path>. Recieved " << arguments.size() << "instead." << endl;
		return;
	}

	int grammarID;
	if (!try_deserialize_integer(arguments.front(), &grammarID) || grammars.count(grammarID) == 0)
	{
		cout << "Invalid grammar id '" << arguments.front() << "'" << endl;
		list();
		return;
	}

	string filePath = concatenate({ arguments.begin() + 1, arguments.end() });
	if (filesystem::exists(filePath))
	{
		if (!show_binary_dialog("File already exists. overwrite?", "yes", "cancel"))return;
	}

	ofstream stream{ filePath };
	//stream << grammars.at(grammarID);
	serialize_grammar(stream, grammars.at(grammarID));
	stream.close();

	cout << "Success." << endl;;
}

void cfg_cli::grammar_union(const vector<string> &arguments)
{
	grammar const *left = nullptr, *right = nullptr;
	if (!try_get_two_grammars_from_ids(arguments, &left, &right))return;

	grammar grammarUnion = left->union_with(*right);

	int unionId = grammars.size();
	grammars[unionId] = grammarUnion;

	cout << "Success. ID: " << unionId;
}

void cfg_cli::grammar_concatenation(const std::vector<std::string>& arguments)
{
	grammar const *left = nullptr, *right = nullptr;
	if (!try_get_two_grammars_from_ids(arguments, &left, &right))return;

	grammar grammarConcatenation = left->concatenation_with(*right);

	int concatenationId = grammars.size();
	grammars[concatenationId] = grammarConcatenation;

	cout << "Success. ID: " << concatenationId;
}

void cfg_cli::grammar_kleene_star(const vector<string> &arguments)
{
	grammar const *grammar;
	if (!try_get_grammar_from_id(arguments, &grammar))return;

	class grammar kleeneStar = grammar->kleene_star();
	int starId = grammars.size();

	grammars[starId] = kleeneStar;

	cout << "Success. ID: " << starId << endl;
}

void cfg_cli::grammar_check_empty(const std::vector<std::string> &arguments)const
{
	grammar const *grammar;
	if (!try_get_grammar_from_id(arguments, &grammar))return;

	cout << "Grammar is empty: " << ((grammar->is_empty()) ? "yes" : "no") << endl;
}

void cfg_cli::grammar_chomskify(const std::vector<std::string>& arguments)
{
	grammar const *grammar;
	if (!try_get_grammar_from_id(arguments, &grammar))return;

	if (grammar->is_chomskified())
	{
		if (!show_binary_dialog("Grammar is already chomskified. Chomskify anyway? ", "yes", "cancel"))return;
	}

	class grammar chomskified = *grammar;
	if (!chomskified.try_chomskify())
	{
		cout << "Sorry, failed to chomskify..." << endl;
		return;
	}

	int chomskifiedId = grammars.size();
	//grammars[chomskifiedId] = chomskified;
	grammars.emplace(chomskifiedId, chomskified);

	cout << "Success. ID: " << chomskifiedId << endl;
}

void cfg_cli::add_rule(const std::vector<std::string>& arguments)
{
	if (arguments.size() < 3)
	{
		cout << "Not enough command arguments. Expected: <Grammar ID> <Rule Identifier> <Rule Expression>." << endl;
		return;
	}

	grammar const *grammar;
	if (!try_get_grammar_from_id({ arguments[0] }, &grammar))return;

	string identifier;
	if (
		!try_deserialize_identifier(arguments[1], &identifier) ||
		term::validate_value(identifier) ||
		term::determine_term_type(identifier) != term_type::rule_reference
		)
	{
		cout << "Invalid identifier: '" << arguments[1] << "'" << endl;
		return;
	}
	if (grammar->contains_rule(identifier))
	{
		cout << "Grammar already contains rule '" << identifier << "'" << endl;
		return;
	}

	production rule;
	string ruleText = concatenate({ arguments.begin() + 2, arguments.end() });
	if (!try_deserialize_production_rule(ruleText, &rule))
	{
		cout << "Could not parse production rule '" << ruleText << "'." << endl;
		return;
	}
	if (!grammar->symbols_exist(rule))
	{
		cout << "Grammar doesn't have some symbols in given expression" << endl;
		return;
	}

	class grammar newGrammar = *grammar;
	newGrammar.add_rule(identifier, rule);

	int newId = grammars.size();
	grammars[newId] = newGrammar;

	cout << "Success. Grammar ID: " << newId << endl;
}

void cfg_cli::remove_rule(const std::vector<std::string>& arguments)
{
	if (arguments.size() != 2)
	{
		cout << "Expected two arguments: <Grammar ID> <Rule Name>. Got " << arguments.size() << " instead." << endl;
		return;
	}

	grammar const *grammar;
	if (!try_get_grammar_from_id({ arguments[0] }, &grammar))return;

	string identifier;
	if (!try_deserialize_identifier(arguments[1], &identifier))
	{
		cout << "Invalid identifier '" << arguments[1] << "' " << endl;
		return;
	}
	if (!grammar->contains_rule(identifier))
	{
		cout << "Grammar does not contain identifier '" << identifier << "' " << endl;
		return;
	}

	class grammar newGrammar = *grammar;
	newGrammar.remove_rule(identifier);

	int newId = grammars.size();
	grammars[newId] = newGrammar;

	cout << "Success. Grammar ID: " << newId << endl;
}

void cfg_cli::cyk(const vector<string> &arguments) const
{
	if (arguments.size() < 2)
	{
		cout << "Not enough arguments. Expected: <Grammar ID> <Search string (Space-delimeted)>" << endl;
		return;
	}

	grammar const *grammar;
	if (!try_get_grammar_from_id({ arguments[0] }, &grammar))return;

	if (!grammar->is_chomskified())
	{
		cout << "Grammar is not in chomsky normal form. Chomskify first." << endl;
		return;
	}

	vector<string> searchString = { arguments.begin() + 1, arguments.end() };

	cyk_parse_tree parseTree;
	if (grammar->try_match(searchString, &parseTree))
	{
		if (show_binary_dialog("Search string has been recognized by the grammar. Show parse tree?", "yes", "no"))
		{
			cout << "Parse tree: " << parseTree << endl;
		}
	}
	else
	{
		cout << "Search string was not recognized by the grammar." << endl;
	}
}

void cfg_cli::grammar_check_chomsky(const std::vector<std::string>& arguments)const
{
	grammar const *grammar;
	if (!try_get_grammar_from_id(arguments, &grammar))return;

	cout << "Grammar is in chomsky normal form: " << ((grammar->is_chomskified()) ? "yes" : "no") << endl;
}

bool cfg_cli::try_get_grammar_from_id(const std::vector<std::string>& arguments, grammar const ** outGrammar)const
{
	int id;

	bool argumentsValid =
		arguments.size() == 1 &&
		try_deserialize_integer(arguments.front(), &id);

	if (!argumentsValid)
	{
		cout << "Expected one argument: <Grammar ID>. Got '" << concatenate(arguments, " ") << "' instead." << endl;
		return false;
	}

	bool validIds = grammars.count(id) != 0;
	if (!validIds)
	{
		cout << "Invalid grammar ID." << endl;
		list();
		return false;
	}

	*outGrammar = &grammars.at(id);
	return true;
}

bool cfg_cli::try_get_two_grammars_from_ids(const vector<string>& arguments, grammar const **outLeft, grammar const **outRight) const
{
	int id1, id2;

	bool argumentsValid =
		arguments.size() == 2 &&
		try_deserialize_integer(arguments.front(), &id1) &&
		try_deserialize_integer(arguments.back(), &id2);

	if (!argumentsValid)
	{
		cout << "Expected two arguments: <id1> <id2>. Got '" << concatenate(arguments, " ") << "' instead." << endl;
		return false;
	}

	bool validIds =
		grammars.count(id1) != 0 &&
		grammars.count(id2) != 0;
	if (!validIds)
	{
		cout << "Invalid grammar ID." << endl;
		list();
		return false;
	}

	*outLeft = &grammars.at(id1);
	*outRight = &grammars.at(id2);
	return true;
}

bool cfg_cli::show_binary_dialog(const std::string & message, const std::string &yes, const std::string &no)const
{
	cout << message << "[" << yes << "\\" << no << "]:" << endl;

	string yesLower = to_lower(yes), noLower = to_lower(no);

	do
	{
		string answer;
		cin >> answer;
		answer = to_lower(answer);

		if (answer == yesLower)return true;
		if (answer == noLower)return false;

		cout << "Expected answer '" << yes << "' or '" << no << "'. Got '" << answer << "' instead. Please try again: " << endl;
	} while (true);
}


cfg_cli::cfg_cli()
{
}


cfg_cli::~cfg_cli()
{
}
