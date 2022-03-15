#include "Member.h"

namespace sda
{
	void Member::setName(std::string name)
	{
		this->name = name;
	}
	void Member::setData(TYPE data)
	{
		this->data = data;
	}
	std::string Member::getName()
	{
		return this->name;
	}
	Member::TYPE Member::getData()
	{
		return this->data;
	}
	Member::Member(std::string name, TYPE data)
	{
		this->name = name;
		this->data = data;
	}
	Member::Member()
	{
		this->name = "EMPTY";
		this->data = TYPE();
	}
}