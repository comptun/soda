#ifndef BYTECODE_INTERPRETER_H
#define BYTECODE_INTERPRETER_H

#include <variant>
#include <vector>
#include <string>

#include "Reference.h"
#include "Byte.h"
#include "Exceptions.h"
#include "Name.h"

namespace sda
{
	class BytecodeInterpreter : public Exceptions
	{
	private:

		typedef long long int INT; // Definitions for the main 3 "types" in soda
		typedef long double FLOAT;
		typedef std::string STRING;

		typedef std::variant<INT, FLOAT, STRING> LIST_TYPE;
		typedef std::vector<LIST_TYPE> LIST;
		typedef std::variant<INT, FLOAT, STRING, Reference, LIST> TYPE;
		typedef std::vector<std::vector<TYPE>> STACK;
		typedef std::vector<std::vector<Name>> NAMES;

		STACK stack;
		NAMES names;
		
		std::string getType(std::string const& str);

		Name& getName(std::string const& name);

		void newStack();
		void popStack();
		void newNames();
		void popNames();

		void push(std::string const& data); // Pushes given data onto the stack
		void pop(); // "Pops" and frees data from the stack
		void var(std::string const& name);
		void pushref(std::string const& ref);
		void pushbackref();
		void assign();

		void add();
		void sub();
		void mul();
		void div();
		void mod();

	public:

		STACK getStack();
		void interpret(Bytecode& bytecode);
	};
}

#endif