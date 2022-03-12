#ifndef PRECOMPILED_FUNCTIONS_H
#define PRECOMPILED_FUNCTIONS_H

#include "Reference.h"

#include <string>
#include <vector>
#include <variant>

namespace sda
{
	class PrecompiledFunctions
	{
	private:

		typedef long long int INT; // Definitions for the main 3 "types" in soda
		typedef long double FLOAT;
		typedef std::string STRING;

		typedef std::variant<INT, FLOAT, STRING, Reference> LIST_TYPE;
		typedef std::vector<LIST_TYPE> LIST;
		typedef std::variant<INT, FLOAT, STRING, Reference, LIST> TYPE;
		typedef std::vector<TYPE> PARAMS;

		TYPE RETURN;

		void putchar(INT out);
		void putstr(STRING out);
		void pause();
		void print(PARAMS& stack);
		void endl();
		void input(PARAMS& stack);

	public:
		TYPE getReturnValue();
		bool call(std::string const& name, PARAMS& stack);
	};
}

#endif
