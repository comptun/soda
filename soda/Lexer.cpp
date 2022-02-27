#include "Lexer.h"
#include "Token.h"

namespace sda
{
	void Lexer::push_token(Token token)
	{
		this->tokens.push_back(token);
	}
	void Lexer::push_token(std::string name, std::string type)
	{
		this->tokens.push_back(Token(name, type));
	}
	Token& Lexer::at(size_t index)
	{
		return this->tokens.at(index);
	}

	int Lexer::readFile(std::string filePath)
	{

	}

	Lexer::Lexer()
	{
		this->possibleTokens = std::vector<Token>{
			Token("function", "functiondef"),
			Token("=", "assign"),
			Token(";", "semicolon"),
			Token("(", "lbracket"),
			Token(")", "rbracket"),
			Token("{", "lcurlybracket"),
			Token("}", "rcurlybracket"),
			Token("+", "add"),
			Token("-", "subtract"),
			Token("*", "multiply"),
			Token("/", "divide"),
			Token("%", "modulus"),
		};
	}
}