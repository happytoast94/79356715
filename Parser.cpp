#include "Parser.h"

Parser::Parser(){}

int Parser::add_file(std::string _filename)
{
	if (!Lexer::getInstance().add_file(_filename))
	{
		return 0;
	}
	tree.define_root(sourcefile());
	tree.print_preorder();
}

int Parser::accept(Symboltable::Symbol s)
{
	if (Lexer::getInstance().sym == s) {
		Lexer::getInstance().next_sym();
		return 1;
	}
	return 0;
}

int Parser::expect(Symboltable::Symbol s)
{
	if (accept(s))
		return 1;
	Lexer::getInstance().error("expected: '" + Symboltable::string_sym[s] + "', found '" + Symboltable::string_sym[Lexer::getInstance().sym] + "'");
	return 0;
}

// TopLevelDecl  = Declaration | FunctionDecl | MethodDecl
// Declaration   = ConstDecl | TypeDecl | VarDecl .
void Parser::topleveldecl()
{

}

// ImportPath = string_lit 
Node* Parser::importpath()
{
	return expect_terminal("ImportPath", Symboltable::quote);  // "abc"
}

// ImportSpec = ["." | PackageName] ImportPath 
Node* Parser::importspec()
{
	Node* result = new Node("ImportSpec");
	Node* tmp_node;
	if (tmp_node = accept_terminal("Token", Symboltable::dot))
	{
		result->add_child(tmp_node);
		result->add_child(importpath());
		return result;
	}
	else if (tmp_node = accept_terminal("Identifier", Symboltable::ident))
	{
		result->add_child(tmp_node);
		result->add_child(importpath());
		return result;
	}
	else
	{
		result->add_child(importpath());
		return result;
	}
}

// ImportDecl = "import" ( ImportSpec | "(" { ImportSpec ";" } ")" ) 
std::vector<Node*> Parser::importdecl()
{
	std::vector<Node*> ls_results;
	Node* tmp_result;
	while (Node* tmp = accept_terminal("Keyword", Symboltable::import))
	{
		if (Node* tmp2 = accept_terminal("Token", Symboltable::lparen))
		{
			tmp_result = new Node("ImportDecl", tmp, tmp2);
			do
			{
				tmp_result->add_child(importspec());
				tmp_result->add_child(expect_terminal("Token", Symboltable::semicolon));
			} while (!accept_terminal("Token", Symboltable::rparen));
			tmp_result->add_child(new Node("Token \"" + Symboltable::getInstance().string_sym[Symboltable::rparen] + "\""));
		}
		else
		{
			tmp_result = new Node("ImportDecl", tmp, importspec());
		}
		tmp_result->add_child(expect_terminal("Token", Symboltable::semicolon));
		ls_results.push_back(tmp_result);
	}

	return ls_results;
}

// PackageName = identifier
Node* Parser::packagename()
{
	Node* result = new Node("PackageName");
	result->add_child(expect_terminal("Identifier " + Symboltable::getInstance().get_last_identifier(), Symboltable::ident));
	return result;
}

// PackageClause = "package" PackageName
Node* Parser::packageclause()
{
	Node* result = new Node("PackageClause");
	result->add_child(expect_terminal("Keyword", Symboltable::package));
	result->add_child(packagename());
	return result;
}

Node* Parser::accept_terminal(std::string description, Symboltable::Symbol s)
{
	if (accept(s))
	{
		return new Node(description + " \"" + Symboltable::string_sym[s] + "\"");
	}
	else return NULL;
}

Node* Parser::expect_terminal(std::string description, Symboltable::Symbol s)
{
	if (expect(s))
	{
		return new Node(description + " \"" + Symboltable::string_sym[s] + "\"");
	}
	else return NULL;
}

// SourceFile = PackageClause ";" { ImportDecl ";" } { TopLevelDecl ";" }
Node* Parser::sourcefile()
{
	Lexer::getInstance().next_sym();
	Node* root = new Node("Sourcefile", packageclause());
	root->add_child(expect_terminal("Token", Symboltable::semicolon));
	root->add_childs(importdecl());
	return root;
}