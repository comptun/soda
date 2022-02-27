#include "Token.h"

namespace sda
{
	// Getters + Setters for Token class //

	std::string Token::getName()
	{
		return this->name;
	}
	std::string Token::getType()
	{
		return this->type;
	}
	void Token::setName(std::string name)
	{
		this->name = name;
	}
	void Token::setType(std::string type)
	{
		this->type = type;
	}
	Token::Token(std::string name, std::string type)
	{
		this->name = name;
		this->type = type;
	}
	Token::Token()
	{
		this->name = "EMPTY";
		this->type = "EMPTY";
	}
}