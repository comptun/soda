#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>

namespace sda
{
	class Lexer
	{
	private:
		std::vector<Token> possibleTokens;
		std::vector<Token> tokens;
		std::string fileContents;
	public:
		void push_token(Token token);
		void push_token(std::string name, std::string type);
		Token& at(size_t index);
		int readFile(std::string filePath);
		Lexer();
	};
}

#endif