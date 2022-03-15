#ifndef CLASS_H
#define CLASS_H

#include <vector>
#include <string>

#include "Class.h"
#include "Name.h"

namespace sda
{
	class Class
	{
	private:
		std::string name;
		std::vector<Name> members;
	public:
		void addMember(Name member);
		std::string getName();
		std::vector<Name> getMembers();
		Class(std::string name);
		Class();
	};
}

#endif