#ifndef NAME_H
#define NAME_H

#include <string>
#include "Reference.h"

namespace sda
{
	class Name
	{
	private:
		std::string m_Name;
		Reference m_Reference;
	public:
		std::string& name();
		Reference& reference();
		Name(std::string m_Name, Reference m_Reference);
		Name();
	};
}

#endif