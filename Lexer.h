#include <fstream>
#include <iostream>
#include "Error.h"
#include "Symboltable.h"

//Singleton
class Lexer
{
	Lexer();

	//variables
	unsigned int ch_count = 0;
	char ch = 0x00;
	unsigned int line_count = 0;
	std::ifstream current_file;
	std::string line;
	
	//methods
	void read_file_next_line();
	void next_char();


public:
	//Singleton stuff
	static Lexer& getInstance()
	{
		static Lexer instance;	// Guaranteed to be destroyed.
		return instance;		// Instantiated on first use.
	}
	Lexer(Lexer const&) = delete;
	void operator=(Lexer const&) = delete;
	~Lexer();

	Symboltable::Symbol sym;

	//methods
	int add_file(std::string filename);
	void next_sym();
	void error(std::string msg);
};