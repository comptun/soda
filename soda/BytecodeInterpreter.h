#ifndef BYTECODE_INTERPRETER_H
#define BYTECODE_INTERPRETER_H

#include <variant>
#include <vector>
#include <string>
#include "Reference.h"
#include "Byte.h"
#include "Exceptions.h"

namespace sda
{
	class BytecodeInterpreter : public Exceptions
	{
	private:

		typedef long long int INT;
		typedef long double FLOAT;
		typedef std::string STRING;
		typedef std::variant<INT, FLOAT, STRING> LIST_TYPE;
		typedef std::vector<LIST_TYPE> LIST;
		typedef std::variant<INT, FLOAT, STRING, LIST> REFTYPE;
		typedef std::variant<INT, FLOAT, STRING, Reference, LIST> TYPE;
		typedef std::vector<TYPE> STACK;

		STACK stack;
		
		std::string getType(std::string const& str);

		void push(std::string const& data); // Pushes given data onto the stack

	public:

		void interpret(Bytecode& bytecode);
	};
}

#endif