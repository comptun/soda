#ifndef MEMBER_H
#define MEMBER_H

#include <vector>
#include <variant>
#include <string>

#include "Reference.h"

namespace sda
{
	class Member
	{
	private:

		typedef long long int INT; // Definitions for the main 3 "types" in soda
		typedef long double FLOAT;
		typedef std::string STRING;

		typedef std::variant<INT, FLOAT, STRING, Reference> LIST_TYPE;
		typedef std::vector<LIST_TYPE> LIST;
		typedef std::variant<INT, FLOAT, STRING, Reference, LIST> TYPE;

		std::string name;
		TYPE data;

	public:

		void setName(std::string name);
		void setData(TYPE data);
		std::string getName();
		TYPE getData();
		Member(std::string name, TYPE data);
		Member();
	};
}

#endif