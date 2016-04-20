#include "Lexer.h"

const std::string Lexer::string_sym[] = { 
	"package", "import", "ident", "+", "-", "*", "/", "%", "+=", "-=",
	"*=", "/=", "%=", "&", "&&", "|", "||",
	"==", "<", ">", ">=", "<=", "!=",
	"(", ")", "[", "]", "{", "}",
	"number", "=", "EOL", "eof", ";", ".", "\"" };

void Lexer::add_file(std::string _filename)
{
	line_count = 0;
	ch_count = 0;
	current_file.open(_filename);

	if (!current_file)
	{
		std::cout << "Could not open file" << std::endl;
		return;
	}

	sourcefile();
	
	current_file.close();
}

void Lexer::read_file_next_line()
{
	if (!std::getline(current_file, line))
	{
		sym = eof;
	}
	line_count++;
	ch_count = 0;
	next_char();
}

void Lexer::error(std::string s)
{
	//output: "[line]: error"
	flag_error = true;
	list_error.push_back(static_cast<std::ostringstream*>(&(std::ostringstream() << line_count))->str() + ": " + s);
}

void Lexer::print_errors()
{
	if (flag_error || !list_error.empty())
	{
		for (int i = 0; i < list_error.size(); ++i)
		{
			std::cout << list_error.at(i) << std::endl;
		}
	}
}

int Lexer::accept(Symbol s)
{
	if (sym == s) {
		next_sym();
		return 1;
	}
	return 0;
}

int Lexer::expect(Symbol s)
{
	if (accept(s))
		return 1;
	error("expected: '" + string_sym[s] + "', found '" + string_sym[sym] + "'");
	return 0;
}


// sets the global variable ch to the next character, if it exists
// increases the global variable chcount
void Lexer::next_char()
{
	if (ch_count < line.size())
		ch = line.at(ch_count);
	else
	{
		ch = 0x00;	//EOL	
	}

	ch_count++;
		
}

// sets the global variable sym to the next symbol, and
// if it is an operator
//   sets the global variable sym to the kind of operator
void Lexer::next_sym()
{
	while (ch == ' ' || ch == 0x9) // horizontal tab
		next_char();

	switch (ch)
	{
	case '"':
		sym = quote; next_char();
		do
		{
			next_char();
		} while (isalnum(ch));

		if (ch == 0x0)
		{
			error("newline in string");
		}
		else if (ch != '"')
		{
			error("unexpected symbol in stringliteral");
		}
		next_char();
		break;
	case ';':
		sym = semicolon; next_char();
		break;
	case '(':
		sym = lparen; next_char();
		break;
	case ')':
		sym = rparen; next_char();
		break;
	case ',':
		sym = comma; next_char();
		break;
	case '%':
		sym = modulo; next_char();
		break;
	case '+':
		sym = plus;  next_char();
		break;
	case '-':
		sym = minus; next_char();
		break;
	case '*':
		sym = times; next_char();
		break;
	case '/':
		sym = slash; next_char();
		if (ch == '/')
		{
			read_file_next_line();
			next_sym();
		}
		else if (ch == '*')
		{
			next_char();
			while (true)
			{
				if (ch == '*')
				{
					next_char();
					if (ch == '/')
					{
						break;
					}
				}
				if (ch == 0x0)
				{
					if (!std::getline(current_file, line))
					{
						sym = eof;
						break;
					}
					ch_count = 0;
					line_count++;
				}
				next_char();		
			}
		}
		break;
	/*
	case '&':
		sym = and; next_char();
		break;
	case '|':
		sym = or; next_char();
		break;
	*/
	case '=':
		sym = assign; next_char();
		if (ch == '=')
		{
			sym = eql; next_char();
		}
		break;
	case '!':
		sym = eql; next_char();
		if (ch == '=')
		{
			sym = neq; next_char();
		}
		break;
	case '>':
		sym = gtr; next_char();
		if (ch == '=')
		{
			sym = geq; next_char();
		}
		break;
	case '<':
		sym = lss;
		next_char();
		if (ch == '=')
		{
			sym = leq; next_char();
		}
		break;
	case 0x0: 
		read_file_next_line();
		next_sym();
		break;
	default:
	{
		//identifier: [a-zA-Z][a-zA-Z]*
		if (isalpha(ch)) 
		{ 
			std::string IdentifierStr;
			do
			{
				IdentifierStr += ch;
				next_char();
			} while (isalnum(ch));
			
			sym = ident;

			if (IdentifierStr == "package")
				sym = package;
			if (IdentifierStr == "import")
				sym = import;
		}

		/*
		// Number: [0-9.]+
		if (isdigit(ch) || ch == '.')
		{
			std::string NumStr;
			do 
			{
				NumStr += ch;
				next_char();
			} while (isdigit(ch) || ch == '.');

			NumVal = strtod(NumStr.c_str(), nullptr);
			return tok_number;
		}
		*/

	}
	}
}

// TopLevelDecl  = Declaration | FunctionDecl | MethodDecl
// Declaration   = ConstDecl | TypeDecl | VarDecl .
void Lexer::topleveldecl()
{

}

// ImportPath = string_lit 
void Lexer::importpath()
{
	expect(quote);  // "abc"
}

// ImportSpec = ["." | PackageName] ImportPath 
void Lexer::importspec()
{
	if (accept(dot))
	{
		importpath();
	}
	else if (accept(ident))
	{
		importpath();
	}
	else importpath();
}

// ImportDecl = "import" ( ImportSpec | "(" { ImportSpec ";" } ")" ) 
void Lexer::importdecl()
{
	if (accept(import))
	{
		if (accept(lparen))
		{		
			importspec();
			accept(semicolon);
			expect(rparen);
		}
		else
		{
			importspec();
		}
	}
}

// PackageClause  = "package" PackageName
// PackageName    = identifier
void Lexer::packageclause()
{
	expect(package);
	expect(ident);
}

// SourceFile = PackageClause ";" { ImportDecl ";" } { TopLevelDecl ";" }
void Lexer::sourcefile()
{
	next_sym();
	packageclause();
	accept(semicolon);
	importdecl();
	//topleveldecl();
}