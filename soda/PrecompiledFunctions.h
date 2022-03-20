#ifndef PRECOMPILED_FUNCTIONS_H
#define PRECOMPILED_FUNCTIONS_H

#include "Reference.h"
#include "Object.h"
#include "Lexer.h"
#include "Translator.h"

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

		typedef std::variant<INT, FLOAT, STRING, Object, Reference> LIST_TYPE;
		typedef std::vector<LIST_TYPE> LIST;
		typedef std::variant<INT, FLOAT, STRING, Object, Reference, LIST> TYPE;
		typedef std::vector<TYPE> PARAMS;
		typedef std::vector<std::vector<TYPE>> STACK;

		TYPE RETURN;

		void putchar(INT out);
		void putstr(STRING out);
		void pause();
		void print(PARAMS& params);
		void endl();
		void input(PARAMS& params);
		void list(PARAMS& params, STACK& stack);
		void rand(INT lowerBound, INT upperBound);
		void disassemble(STRING str);
		void bytecode(LIST bc);

	public:
		TYPE getReturnValue();
		bool call(std::string const& name, PARAMS& params, STACK& stack);
	};
}

#endif
