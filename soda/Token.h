#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace sda
{
	class Token
	{
	private:
		std::string name;
		std::string type;
	public:
		void setName(std::string name);
		void setType(std::string type);
		std::string getName();
		std::string getType();
		Token(std::string name, std::string type);
		Token();
	};
}

#endif
