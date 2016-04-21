#include "Lexer.h"

const std::string Lexer::string_sym[] = {
	"package", "import", "ident", "+", "-", "*", "/", "%", "+=", "-=",
	"*=", "/=", "%=",
	"&", "&&", "|", "||",
	"==", "<", ">", ">=", "<=", "!=",
	"(", ")", "[", "]", "{", "}",
	"number", "=", "EOL", "eof", ";", ".", "\"" };

void Lexer::add_file(std::string _filename)
{
	line_count = 0;
	ch_count = 0;
	current_file.open(_filename, std::fstream::in);

	if (!current_file.is_open())
	{
		std::cout << "Could not open file " + _filename << std::endl;
		return;
	}

	sourcefile();
	
	current_file.close();
}

void Lexer::read_file_next_line()
{
	line = "";

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
	
	std::string str_error = "";
	str_error += static_cast<std::ostringstream*>(&(std::ostringstream() << line_count))->str() + ": " + s + "\n";
	str_error += line + "\n";
	
	for (int i = 2; i < ch_count; ++i)
		str_error += " ";
	str_error += "^";

	list_error.push_back(str_error);
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
	else if (sym != eof)
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
			while (true)
			{
				next_char();
				if (ch == '*')
				{
					while (ch == '*')
					{
						next_char();
					}
					
					if (ch == '/')
					{
						break;
					}
				}
				if (ch == 0x0)
				{
					line = "";

					if (!std::getline(current_file, line))
					{
						sym = eof;
						error("comment doesnt end");
						break;
					}

					line_count++;
					ch_count = 0;
				}	
			}
			next_char();
			next_sym();
		}
		break;
	case '&':
		sym = and_sym; next_char();
		break;
	case '|':
		sym = or_sym; next_char();
		break;
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
		if (sym == eof)
			break;

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
	while (accept(import))
	{
		if (accept(lparen))
		{	
			do
			{
				importspec();
				expect(semicolon);
			} while (!accept(rparen));
		}
		else
		{
			importspec();
		}
		expect(semicolon);
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
	expect(semicolon);
	importdecl();
	//topleveldecl();
}