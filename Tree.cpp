#include "Tree.h"

Tree::Tree()
{
	root = NULL;
}

Tree::~Tree()
{
	destroy_tree();
}

void Tree::destroy_tree()
{
	destroy_tree(root);
}

void Tree::destroy_tree(Node *leaf)
{
	if (leaf != NULL)
	{
		for (unsigned int i = 0; i < leaf->children.size(); ++i)
		{
			destroy_tree(leaf->children.at(i));
		}
		delete leaf;
		leaf = NULL;
	}
}

void Tree::define_root(Node* n)
{
	root = n;
}

/*
Display the data part of the root(or current node).
Traverse the left subtree by recursively calling the pre - order function.
Traverse the right subtree by recursively calling the pre - order function.
*/
void Tree::print_preorder()
{
	print_preorder(root, 0);
}

void Tree::print_preorder(Node* n, int level)
{
	std::string indent = "";
	for (int i = 0; i < level; ++i)
		indent += "   ";
	indent += "|__";

	std::cout << indent;

	n->print();
	for (unsigned int i = 0; i < n->children.size(); ++i)
		print_preorder(n->children.at(i), level+1);
}