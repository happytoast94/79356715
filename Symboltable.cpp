#include "Symboltable.h"
#include <iostream>
const std::string Symboltable::string_sym[] =
{
	"package", "import", "identifier", "+", "-", "*", "/", "%", "+=", "-=",
	"*=", "/=", "%=",
	"&", "&&", "|", "||",
	"==", "<", ">", ">=", "<=", "!=",
	"(", ")", "[", "]", "{", "}",
	"number", "=", "EOL", "eof", ";", ".", "string_literal"
};

void Symboltable::add_identifier(std::string id)
{
	int size = ls_identifier.size();
	ls_identifier.push_back(std::make_pair(size, id));
}

std::string Symboltable::get_last_identifier()
{
	if (ls_identifier.size() != 0)
		return ls_identifier.at(ls_identifier.size()-1).second;
	else return "error";
}