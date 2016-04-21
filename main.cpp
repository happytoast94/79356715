#include "Parser.h"

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

	Parser p;
	
	for (std::string s : files)
	{
		
		p.add_file(s);
	}
	

	Error::getInstance().print_errors();
}