#include "Parser.h"

Parser::Parser(){}

void Parser::add_file(std::string _filename)
{
	Lexer::getInstance().add_file(_filename);
	sourcefile();
}

int Parser::accept(Symboltable::Symbol s)
{
	if (Lexer::getInstance().sym == s) {
		Lexer::getInstance().next_sym();
		return 1;
	}
	return 0;
}

int Parser::expect(Symboltable::Symbol s)
{
	if (accept(s))
		return 1;
	Lexer::getInstance().error("expected: '" + Symboltable::string_sym[s] + "', found '" + Symboltable::string_sym[Lexer::getInstance().sym] + "'");
	return 0;
}

// TopLevelDecl  = Declaration | FunctionDecl | MethodDecl
// Declaration   = ConstDecl | TypeDecl | VarDecl .
void Parser::topleveldecl()
{

}

// ImportPath = string_lit 
void Parser::importpath()
{
	expect(Symboltable::quote);  // "abc"
}

// ImportSpec = ["." | PackageName] ImportPath 
void Parser::importspec()
{
	if (accept(Symboltable::dot))
	{
		importpath();
	}
	else if (accept(Symboltable::ident))
	{
		importpath();
	}
	else importpath();
}

// ImportDecl = "import" ( ImportSpec | "(" { ImportSpec ";" } ")" ) 
void Parser::importdecl()
{
	while (accept(Symboltable::import))
	{
		if (accept(Symboltable::lparen))
		{
			do
			{
				importspec();
				expect(Symboltable::semicolon);
			} while (!accept(Symboltable::rparen));
		}
		else
		{
			importspec();
		}
		expect(Symboltable::semicolon);
	}
}

// PackageClause  = "package" PackageName
// PackageName    = identifier
void Parser::packageclause()
{
	expect(Symboltable::package);
	expect(Symboltable::ident);
}

// SourceFile = PackageClause ";" { ImportDecl ";" } { TopLevelDecl ";" }
void Parser::sourcefile()
{
	Lexer::getInstance().next_sym();
	packageclause();
	expect(Symboltable::semicolon);
	importdecl();
	//topleveldecl();
}