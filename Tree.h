#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <iostream>

struct Node
{
	std::string description;
	std::vector<Node*> children;
	
	Node(){}

	Node(std::string _description) { description = _description; }

	Node(std::string _description, Node* a)
	{
		description = _description;
		children.push_back(a);
	}

	Node(std::string _description, Node* a, Node* b)
	{
		description = _description;
		children.push_back(a);
		children.push_back(b);
	}

	Node(std::string _description, Node* a, Node* b, Node* c)
	{
		description = _description;
		children.push_back(a);
		children.push_back(b);
		children.push_back(c);
	}

	Node(std::string _description, std::vector<Node*> ls_nodes)
	{
		description = _description;
		for (unsigned int i = 0; i < ls_nodes.size(); ++i)
		{
			children.push_back(ls_nodes.at(i));
		}
	}

	void add_child(Node* n)
	{
		children.push_back(n);
	}

	void add_childs(std::vector<Node*> ls_nodes)
	{
		for (unsigned int i = 0; i < ls_nodes.size(); ++i)
		{
			children.push_back(ls_nodes.at(i));
		}
	}

	void print()
	{
		std::cout << description << std::endl;
	}
};

class Tree
{
public:
	Tree();
	~Tree();

	void destroy_tree();
	void define_root(Node* n);
	void print_preorder();
	void print_preorder(Node* n, int level);

private:
	void destroy_tree(Node *leaf);
	Node *root;
};

#endif