#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include "Class.h"
#include "Name.h"
#include "Member.h"
#include "Reference.h"

namespace sda
{
	class Object 
	{
	private:
		std::vector<Name> members;
		Class cls;
	public:
		std::vector<Name>& getMembers();
		Name& getMember(size_t index);
		Class getClass();
		Object(Class cls);
	};
}

#endif