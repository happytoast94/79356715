#ifndef ERROR_H
#define ERROR_H

#include <vector>
#include <string>

class Error
{
	Error(){}
	~Error(){}
	std::vector<std::string> list_error;


public:
	//Singleton stuff
	static Error& getInstance()
	{
		static Error instance;	// Guaranteed to be destroyed.
		return instance;		// Instantiated on first use.
	}
	Error(Error const&) = delete;
	void operator=(Error const&) = delete;

	void add_error_msg(std::string msg, unsigned int line_number, unsigned int pos, std::string line);
	void print_errors();

};

#endif