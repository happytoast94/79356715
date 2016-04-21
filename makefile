all:
	g++ main.cpp Lexer.h Lexer.cpp Parser.h Parser.cpp Error.h Error.cpp Symboltable.h Symboltable.cpp -o out -std=c++11 