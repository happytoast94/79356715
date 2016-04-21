#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>

class Symboltable
{

public:
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
};

#endif


