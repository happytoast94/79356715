#include "Lexer.h"
#include "Symboltable.h"
#include "Tree.h"

class Parser
{
	Tree tree;

	// methods
	Node* sourcefile();
	Node* packageclause();
	Node* packagename();
	std::vector<Node*> importdecl();
	Node* importspec();
	Node* importpath();
	void topleveldecl();

	Node* accept_terminal(std::string description, Symboltable::Symbol s);
	Node* expect_terminal(std::string description, Symboltable::Symbol s);

	int accept(Symboltable::Symbol s);
	int expect(Symboltable::Symbol s);

public:
	Parser();

	int add_file(std::string filename);

};