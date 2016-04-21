#include "Lexer.h"

int main(int argc, char *argv[])
{	
	std::vector<std::string> files;		//List of files
	
	if (argc > 1) 
	{
		files.assign(argv + 1, argv + argc);
	}
	else
	{
		std::cout << "Enter a Go-File name" << std::endl;
	}

	for (std::string s : files)
	{
		Lexer::getInstance().add_file(s);
	}
	
	if (Lexer::getInstance().flag_error)
	{
		Lexer::getInstance().print_errors();
	}
	else std::cout << "All good" << std::endl;
	
}