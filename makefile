all:
	g++ main.cpp Lexer.cpp Parser.cpp Error.cpp Symboltable.cpp Tree.cpp -g -o out -std=c++11 
