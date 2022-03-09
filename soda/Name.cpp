#include "Name.h"

namespace sda
{
	std::string& Name::name()
	{
		return this->m_Name;
	}
	Reference& Name::reference()
	{
		return this->m_Reference;
	}
	Name::Name(std::string m_Name, Reference m_Reference)
	{
		this->m_Name = m_Name;
		this->m_Reference = m_Reference;
	}
	Name::Name()
	{
		this->m_Name = std::string();
		this->m_Reference = Reference();
	}
}