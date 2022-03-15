#include "Object.h"

namespace sda
{
	std::vector<Name>& Object::getMembers()
	{
		return this->members;
	}
	Name& Object::getMember(size_t index)
	{
		return this->members.at(index);
	}

	Object::Object(Class cls)
	{
		this->cls = cls;
		this->members = this->cls.getMembers();
	}
}