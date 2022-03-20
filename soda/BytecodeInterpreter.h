#ifndef BYTECODE_INTERPRETER_H
#define BYTECODE_INTERPRETER_H

#include <variant>
#include <vector>
#include <string>

#include "Reference.h"
#include "Byte.h"
#include "Exceptions.h"
#include "Name.h"
#include "PrecompiledFunctions.h"
#include "Class.h"
#include "Object.h"

namespace sda
{
	class BytecodeInterpreter : public Exceptions, public PrecompiledFunctions
	{
	private:

		typedef long long int INT; // Definitions for the main 3 "types" in soda
		typedef long double FLOAT;
		typedef std::string STRING;

		typedef std::vector<Reference> LIST;
		typedef std::variant<INT, FLOAT, STRING, Object, Reference, LIST> TYPE;
		typedef std::vector<std::vector<TYPE>> STACK;
		typedef std::vector<TYPE> HEAP;
		typedef std::vector<std::vector<Name>> NAMES;
		typedef std::vector<size_t> JUMPSTACK;
		typedef std::vector<Class> CSTACK;

		CSTACK cstack; // stores class definitions
		STACK stack;
		STACK params;
		NAMES names;
		JUMPSTACK js;

		TYPE RETURN;
		
		std::string getType(std::string const& str);

		Name& getName(std::string const& name);
		TYPE& getNameValue(std::string const& name);
		Class getClass(std::string const& name);

		void newstack();
		void popstack();
		void newnames();
		void popnames();

		void newparamstack();
		void popparamstack();

		TYPE& deref(Reference ref);
		TYPE& tracebackReference(TYPE& ref);
		void push(std::string const& data); // Pushes given data onto the stack
		void pop(); // "Pops" and frees data from the stack
		void pushparam(); // Pushes parameter from regular stack onto paramater stack for usage in functions
		void var(std::string const& name);
		void pushref(std::string const& ref);
		void pushbackref();
		void assign();
		void pushname(std::string const& name);
		void pushlistindex();
		void pushnewobject(std::string const& className);
		void pushmember(std::string const& name);

		void add();
		void sub();
		void mul();
		void div();
		void mod();
		void bitwiseand();
		void bitwiseor();
		void bitwisexor();
		void lshift();
		void rshift();

	public:

		STACK getStack();
		void interpret(Bytecode& bytecode);
	};
}

#endif