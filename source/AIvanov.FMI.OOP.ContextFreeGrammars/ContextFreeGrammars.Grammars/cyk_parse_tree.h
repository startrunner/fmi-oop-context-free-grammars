#pragma once
#include "stdafx.h"

class cyk_parse_tree_node
{
	std::string name;
	std::vector<cyk_parse_tree_node> children;
public:
	cyk_parse_tree_node() :
		cyk_parse_tree_node("") {}

	cyk_parse_tree_node(const std::string &name) :
		cyk_parse_tree_node(name, {}) {}

	cyk_parse_tree_node(const std::string &name, const std::vector<cyk_parse_tree_node> &children) :
		name(name), children(children) {}

	cyk_parse_tree_node& operator = (const cyk_parse_tree_node &other);

	std::string get_name()const;
	void set_name(const std::string &name);

	int child_count()const;
	cyk_parse_tree_node& child(int index);
	const cyk_parse_tree_node& child(int index)const;
	void set_child(const cyk_parse_tree_node &node, int index);
	void add_child(const cyk_parse_tree_node &node);
	void clear_children();

	bool operator < (const cyk_parse_tree_node &other)const;
	bool operator == (const cyk_parse_tree_node &other)const;

	friend std::ostream& operator <<(std::ostream &stream, const cyk_parse_tree_node &node);
private:
	void print(std::ostream &stream, int indentation = 0)const;
	bool is_terminal()const;
};

class cyk_parse_tree
{
	cyk_parse_tree_node root;

public:
	cyk_parse_tree() {}
	cyk_parse_tree(const cyk_parse_tree_node &root) :root(root) {/* i am groot*/ }

	cyk_parse_tree& operator =(const cyk_parse_tree &other) { root = other.root; return *this; }
	bool operator == (const cyk_parse_tree &other)const { return root == other.root; }
	bool operator < (const cyk_parse_tree &other)const { return root < other.root; }

	friend std::ostream& operator <<(std::ostream &stream, const cyk_parse_tree &tree);
};
