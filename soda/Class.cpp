#include "Class.h"

namespace sda
{
	void Class::addMember(Name member)
	{
		members.push_back(member);
	}
	std::string Class::getName()
	{
		return this->name;
	}
	std::vector<Name> Class::getMembers()
	{
		return this->members;
	}
	Class::Class(std::string name)
	{
		this->name = name;
	}
	Class::Class()
	{
		this->name = "EMPTY";
	}
}