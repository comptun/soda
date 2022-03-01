#include "Token.h"
#include "TokenTypes.h"

#include <iostream>

namespace sda
{
	// Getters + Setters for Token class //

	std::string Token::getName()
	{
		return this->name;
	}
	TT Token::getType()
	{
		return this->type;
	}
	void Token::setName(std::string name)
	{
		this->name = name;
	}
	void Token::setType(TT type)
	{
		this->type = type;
	}
	bool operator==(Token lhs, Token rhs)
	{
		return lhs.getName() == rhs.getName() && lhs.getType() == rhs.getType();
	}
	bool operator!=(Token lhs, Token rhs)
	{
		return !(lhs == rhs);
	}
	std::ostream& operator<<(std::ostream& out, std::vector<Token>& list)
	{
		for (Token& i : list) {
			std::cout << i.getName() << " " << static_cast<int>(i.getType()) << "\n";
		}
		return out;
	}
	Token::Token(std::string name, TT type)
	{
		this->name = name;
		this->type = type;
	}
	Token::Token()
	{
		this->name = "EMPTY";
		this->type = TT::EMPTY;
	}
}