#include "stdafx.h"
#include "cyk_parse_tree.h"
using namespace std;

cyk_parse_tree_node &cyk_parse_tree_node::operator=(const cyk_parse_tree_node &other)
{
	if (this == &other)return *this;

	name = other.name;
	children = other.children;

	return *this;
}

string cyk_parse_tree_node::get_name() const { return name; }

void cyk_parse_tree_node::set_name(const string &name) { this->name = name; }

int cyk_parse_tree_node::child_count()const { return children.size(); }

cyk_parse_tree_node& cyk_parse_tree_node::child(int index) { return children[index]; }

const cyk_parse_tree_node& cyk_parse_tree_node::child(int index) const { return children[index]; }

void cyk_parse_tree_node::set_child(const cyk_parse_tree_node &node, int index) { children[index] = node; }

void cyk_parse_tree_node::add_child(const cyk_parse_tree_node &node) { children.push_back(node); }

void cyk_parse_tree_node::clear_children() { children.clear(); }

bool cyk_parse_tree_node::operator<(const cyk_parse_tree_node &other) const
{
	if (this == &other)return false;
	return name < other.name || (name == other.name && children < other.children);
}

bool cyk_parse_tree_node::operator==(const cyk_parse_tree_node &other) const
{
	if (this == &other)return true;
	return name == other.name && children == other.children;
}

ostream& operator<<(ostream &stream, const cyk_parse_tree &tree) { return stream << tree.root; }

ostream& operator<<(ostream &stream, const cyk_parse_tree_node &node) { node.print(stream); return stream; }

ostream& indent(ostream &stream, int indentation);

void cyk_parse_tree_node::print(ostream &stream, int indentation)const
{
	if (child_count() == 1 && child(0).is_terminal())
	{
		indent(stream, indentation) << get_name() << " -> " << child(0).get_name();
	}
	else
	{
		indent(stream, indentation) << get_name() << "{" << endl;
		for (int i = 0; i < child_count(); i++)
		{
			child(i).print(stream, indentation + 1);
			stream << "," << endl;
		}
		indent(stream, indentation) << "}";
	}
}

bool cyk_parse_tree_node::is_terminal() const { return children.empty(); }

ostream& indent(ostream &stream, int indentation)
{
	for (int i = 0; i < indentation; i++)stream << ' ';
	return stream;
}