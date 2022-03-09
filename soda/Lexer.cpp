#include "Lexer.h"
#include "Token.h"
#include "TokenTypes.h"

#include <fstream>

namespace sda
{
	void Lexer::push_token(Token token)
	{
		this->tokens.push_back(token);
	}
	void Lexer::push_token(std::string name, TT type)
	{
		this->tokens.push_back(Token(name, type));
	}
	Token& Lexer::at(size_t index)
	{
		return this->tokens.at(index);
	}

	void Lexer::readFile(std::string const& filePath)
	{
		std::fstream file(filePath);
		std::string line;
		while (std::getline(file, line)) {
			if (this->getAhead(line, 0, 6) == "import") {
				std::string linkedFile = this->getAhead(line, 8, line.size() - 1);
				this->readFile(linkedFile);
				continue;
			}
			this->fileContents += (line + " ");
		}
		file.close();
	}

	bool Lexer::isToken(std::string const& token)
	{
		for (Token& i : this->possibleTokens) {
			if (i.getName() == token)
				return true;
		}
		return false;
	}

	bool Lexer::isSpecialCharacter(std::string const& str, size_t size, size_t start)
	{
		std::string chars;
		for (size_t i = start; i < start + size && i < str.size(); ++i) {
			chars.push_back(str.at(i));
		}
		return isToken(chars);
	}

	Token Lexer::getToken(std::string const& name)
	{
		for (Token& i : this->possibleTokens) {
			if (i.getName() == name)
				return i;
		}
		return Token();
	}

	TT Lexer::getTokenType(std::string const& name)
	{
		return this->getToken(name).getType();
	}

	size_t Lexer::largestToken()
	{
		size_t largest = 0;
		for (Token& i : this->possibleTokens) {
			if (i.getName().size() >= largest) {
				largest = i.getName().size();
			}
		}
		return largest;
	}

	std::string Lexer::getAhead(std::string const& str, size_t start, size_t end)
	{
		std::string inbetween;
		for (size_t i = start; i < end && i < str.size(); ++i) {
			inbetween.push_back(str.at(i));
		}
		return inbetween;
	}

	bool Lexer::isWhitespace(char c)
	{
		return c == ' ' || c == '	';
	}

	bool Lexer::isNumber(std::string const& str)
	{
		bool periodFound = false;
		for (auto& i : str) {
			if (i == '.' && !periodFound) {
				periodFound = true;
			}
			else if ((i == '.' && periodFound) || !std::isdigit(i)) {
				return false;
			}
		}
	}

	Token Lexer::getSpecialToken(std::string const& str, size_t index)
	{
		for (size_t i = this->largestToken(); i > 0; --i) {
			std::string ahead = this->getAhead(str, index, index + i);
			if (this->isToken(ahead)) {
				return this->getToken(ahead);
			}
		}
		return Token();
	}

	Token Lexer::getName(std::string const& str, size_t index)
	{
		std::string name;
		if (str.at(index) == '"') {
			std::string substr;
			for (size_t i = index + 1; str.at(i) != '"'; ++i) {
				substr.push_back(str.at(i));
			}
			return Token(substr, TT::STRING);
		}
		if (std::isdigit(str.at(index))) {
			std::string substr;
			for (size_t i = index; str.at(i) == '.' || std::isdigit(str.at(i)); ++i) {
				substr.push_back(str.at(i));
			}
			return Token(substr, TT::NUM);
		}
		for (size_t i = index; i < str.size() && !this->isWhitespace(str.at(i)) && getSpecialToken(str, i) == Token(); ++i) {
			name.push_back(str.at(i));
		}
		return Token(name, TT::NAME);
	}

	void Lexer::tokenize(std::string const& name)
	{
		if (this->isToken(name)) {
			this->push_token(this->getToken(name));
			return;
		}
		this->push_token(name, TT::NAME);
	}

	void Lexer::lex(std::string const& filePath)
	{
		this->readFile(filePath);
		for (size_t i = 0; i < this->fileContents.size();) {
			Token potentialToken = this->getSpecialToken(this->fileContents, i);
			if (potentialToken != Token()) {
				this->push_token(potentialToken);
				i += potentialToken.getName().size();
				continue;
			}
			else if (!this->isWhitespace(this->fileContents.at(i))) {
				Token potentialName = this->getName(this->fileContents, i);
				this->push_token(potentialName);
				if (potentialName.getType() == TT::STRING) {
					i += potentialName.getName().size() + 2;
					continue;
				}
				i += potentialName.getName().size();
				continue;
			}
			i += 1;
		}
		//std::cout << this->tokens;
	}

	TokenList& Lexer::getTokens()
	{
		return this->tokens;
	}

	Lexer::Lexer()
	{
		using T = Token;
		this->possibleTokens = TokenList{
			T("function", TT::FUNCTION),
			T("var", TT::VAR),
			T("=", TT::ASSIGNMENT),
			T(";", TT::SEMICOLON),
			T("(", TT::LBRACKET),
			T(")", TT::RBRACKET),
			T("{", TT::LCURLYBRACE),
			T("}", TT::RCURLYBRACE),
			T("[", TT::LSQUAREBRACE),
			T("]", TT::RSQUAREBRACE),
			T("+", TT::ADD),
			T("-", TT::SUBTRACT),
			T("*", TT::MULTIPLY),
			T("/", TT::DIVIDE),
			T("%", TT::MODULUS),
			T(".", TT::DOT),
			T(",", TT::COMMA),
			T("+=", TT::PLUSEQUALS),
			T("-=", TT::SUBTRACTEQUALS),
			T("*=", TT::MULTIPLYEQUALS),
			T("%=", TT::MODULUSEQUALS),
			T("::", TT::DOUBLECOLON),
			T("==", TT::EQUALTO),
			T("<", TT::LESSTHAN),
			T(">", TT::GREATERTHAN),
			T("<=", TT::LESSTHANEQUALTO),
			T(">=", TT::GREATERTHANEQUALTO),
			T("!=", TT::NOTEQUALTO),
			T("&&", TT::AND),
			T("||", TT::OR),
			T("!", TT::NOT),
			T("&", TT::BITWISEAND),
			T("|", TT::BITWISEOR),
			T("^", TT::BITWISEXOR),
			T("<<", TT::LEFTSHIFT),
			T(">>", TT::RIGHTSHIFT),
		};
	}
}