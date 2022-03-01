#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>
#include "TokenTypes.h"

namespace sda
{
	class Token
	{
	private:
		std::string name;
		TT type;
	public:
		void setName(std::string name);
		void setType(TT type);
		std::string getName();
		TT getType();
		friend bool operator==(Token lhs, Token rhs);
		friend bool operator!=(Token lhs, Token rhs);
		friend std::ostream& operator<<(std::ostream& out, std::vector<Token>& list);
		Token(std::string name, TT type);
		Token();
	};

	typedef std::vector<Token> TokenList;
}

#endif
