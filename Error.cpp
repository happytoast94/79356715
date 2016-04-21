#include "Error.h"

#include <string>
#include <sstream>
#include <iostream>

Error::Error(){}

void Error::add_error_msg(std::string msg, unsigned int line_number, unsigned int pos, std::string line)
{
	std::string str_error = "";
	str_error += static_cast<std::ostringstream*>(&(std::ostringstream() << line_number))->str() + ": " + msg + "\n";
	str_error += line + "\n";

	for (int i = 2; i < pos; ++i)
		str_error += " ";
	str_error += "^";

	list_error.push_back(str_error);
}

//output: "[line]: error"
void Error::print_errors()
{
	if (!list_error.empty())
	{
		for (int i = 0; i < list_error.size(); ++i)
		{
			std::cout << list_error.at(i) << std::endl;
		}
	}
}