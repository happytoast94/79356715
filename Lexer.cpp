#include "Lexer.h"

Lexer::Lexer(){}
Lexer::~Lexer(){ current_file.close(); }

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
}

void Lexer::read_file_next_line()
{
	line = "";

	if (!std::getline(current_file, line))
	{
		sym = Symboltable::eof;
	}

	line_count++;
	ch_count = 0;
	next_char();
}

// sets the global variable ch to the next character, if it exists
// increases the global variable chcount
void Lexer::next_char()
{
	if (ch_count < line.size())
		ch = line.at(ch_count);
	else if (sym != Symboltable::eof)
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
		sym = Symboltable::quote; next_char();
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
		sym = Symboltable::semicolon; next_char();
		break;
	case '(':
		sym = Symboltable::lparen; next_char();
		break;
	case ')':
		sym = Symboltable::rparen; next_char();
		break;
	case ',':
		sym = Symboltable::comma; next_char();
		break;
	case '%':
		sym = Symboltable::modulo; next_char();
		break;
	case '+':
		sym = Symboltable::plus;  next_char();
		break;
	case '-':
		sym = Symboltable::minus; next_char();
		break;
	case '*':
		sym = Symboltable::times; next_char();
		break;
	case '/':
		sym = Symboltable::slash; next_char();
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
						sym = Symboltable::eof;
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
		sym = Symboltable::and_sym; next_char();
		break;
	case '|':
		sym = Symboltable::or_sym; next_char();
		break;
	case '=':
		sym = Symboltable::assign; next_char();
		if (ch == '=')
		{
			sym = Symboltable::eql; next_char();
		}
		break;
	case '!':
		sym = Symboltable::eql; next_char();
		if (ch == '=')
		{
			sym = Symboltable::neq; next_char();
		}
		break;
	case '>':
		sym = Symboltable::gtr; next_char();
		if (ch == '=')
		{
			sym = Symboltable::geq; next_char();
		}
		break;
	case '<':
		sym = Symboltable::lss;
		next_char();
		if (ch == '=')
		{
			sym = Symboltable::leq; next_char();
		}
		break;
	case 0x0: 
		read_file_next_line();
		if (sym == Symboltable::eof)
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
			
			sym = Symboltable::ident;

			if (IdentifierStr == "package")
				sym = Symboltable::package;
			if (IdentifierStr == "import")
				sym = Symboltable::import;
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

void Lexer::error(std::string msg)
{
	Error::getInstance().add_error_msg(msg, line_count, ch_count, line);
}