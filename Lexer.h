#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

//Singleton
class Lexer
{
	Lexer() {};

	typedef enum
	{
		package, import, ident,
		//operators
		plus, minus, times, slash, modulo, pluseql, minuseql, timeseql, slasheql, moduloeql,
		and, andand, or, oror,
		//comparators
		eql, lss, gtr, geq, leq, neq,
		//brackets
		lparen, rparen, lsqrbr, rsqrbr, lcrlybr, rcrlybr,
		number, assign, EOL, eof, semicolon, dot, quote,
		callsym, beginsym, 
		endsym, ifsym, whilesym, becomes, thensym, dosym, constsym, comma,
		varsym, procsym, period, oddsym
	} Symbol;
	
	const static std::string string_sym[];

	//variables
	Symbol sym;
	unsigned int ch_count = 0;
	char ch = 0x00;
	unsigned int line_count = 0;
	std::ifstream current_file;
	std::string line;
	std::vector<std::string> list_error;


	//methods
	void read_file_next_line();
	void error(std::string s);
	int accept(Symbol s);
	int expect(Symbol s);
	void next_char();
	void next_sym();
	void sourcefile();
	void packageclause();
	void importdecl();
	void importspec();
	void importpath();
	void topleveldecl();

public:
	//Singleton stuff
	static Lexer& getInstance()
	{
		static Lexer instance;	// Guaranteed to be destroyed.
		return instance;		// Instantiated on first use.
	}
	Lexer(Lexer const&) = delete;
	void operator=(Lexer const&) = delete;

	//variables
	bool flag_error = false;


	
	//methods
	void add_file(std::string filename);
	void print_errors();
};