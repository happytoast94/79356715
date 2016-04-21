#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <vector>

class Symboltable
{
	Symboltable(){};

public:
	//Singleton stuff
	static Symboltable& getInstance()
	{
		static Symboltable instance;	// Guaranteed to be destroyed.
		return instance;		// Instantiated on first use.
	}
	Symboltable(Symboltable const&) = delete;
	void operator=(Symboltable const&) = delete;

	typedef enum
	{
		package, import, ident,
		//operators
		plus, minus, times, slash, modulo, pluseql, minuseql, timeseql, slasheql, moduloeql,
		and_sym, andand, or_sym, oror,
		//comparators
		eql, lss, gtr, geq, leq, neq,
		//brackets
		lparen, rparen, lsqrbr, rsqrbr, lcrlybr, rcrlybr,
		number, assign, EOL, eof, semicolon, dot, quote,
		callsym, beginsym,
		endsym, ifsym, whilesym, becomes, thensym, dosym, constsym, comma,
		varsym, procsym, period, oddsym
	} Symbol;

	static const std::string string_sym[];

	std::vector<std::pair<int, std::string>> ls_identifier;
	void add_identifier(std::string id);
	std::string get_last_identifier();
};

#endif


