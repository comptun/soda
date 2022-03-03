#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>

#include "TokenTypes.h"
#include "Token.h"

namespace sda
{
	class Lexer
	{
	private:
		TokenList possibleTokens;
		TokenList tokens;
		std::string fileContents;
		std::string getAhead(std::string const& str, size_t start, size_t end);
		void readFile(std::string const& filePath);
		bool isToken(std::string const& token);
		bool isSpecialCharacter(std::string const& str, size_t size, size_t start);
		Token getToken(std::string const& name);
		TT getTokenType(std::string const& name);
		size_t largestToken();
		bool isWhitespace(char c);
		bool isNumber(std::string const& str);
		Token getSpecialToken(std::string const& str, size_t index);
		Token getName(std::string const& str, size_t index);
		void tokenize(std::string const& name);
	public:
		void push_token(Token token);
		void push_token(std::string name, TT type);
		Token& at(size_t index);
		void lex(std::string const& filePath);
		Lexer();
	};
}

#endif