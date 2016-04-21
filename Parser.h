#include "Lexer.h"
#include "Symboltable.h"

class Parser
{
	// methods
	void sourcefile();
	void packageclause();
	void importdecl();
	void importspec();
	void importpath();
	void topleveldecl();

	int accept(Symboltable::Symbol s);
	int expect(Symboltable::Symbol s);

public:
	Parser();

	void add_file(std::string filename);

};