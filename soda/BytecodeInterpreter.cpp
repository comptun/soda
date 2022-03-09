#include "BytecodeInterpreter.h"
#include "Name.h"

#include <iostream>

namespace sda
{
	std::string BytecodeInterpreter::getType(std::string const& str)
	{
		bool isFloat = false;
		for (size_t i = 0; i < str.size(); ++i) {
			if (!std::isdigit(str.at(i)) && str.at(i) != '.')
				return "string";
			if (str.at(i) == '.')
				isFloat = true;
		}
		if (isFloat)
			return "float";
		return "int";
	}

	Name& BytecodeInterpreter::getName(std::string const& name)
	{
		for (auto n : names.front()) { // Checks global variables
			if (n.name() == name)
				return n;
		}
		if (names.size() > 1) { // Checks if program is beyond global scope
			for (auto n : names.back()) { // Checks variables in local scope
				if (n.name() == name)
					return n;
			}
		}
	}

	void BytecodeInterpreter::newStack() { this->stack.push_back(std::vector<TYPE>()); }
	void BytecodeInterpreter::popStack() { this->stack.pop_back(); }
	void BytecodeInterpreter::newNames() { this->names.push_back(std::vector<Name>()); }
	void BytecodeInterpreter::popNames() { this->names.pop_back(); }

	void BytecodeInterpreter::push(std::string const& data)
	{
		if (this->getType(data) == "int")
			this->stack.back().push_back(std::stoll(data));
		else if (this->getType(data) == "float")
			this->stack.back().push_back(std::stold(data));
		else if (this->getType(data) == "string")
			this->stack.back().push_back(data);
		else
			this->exception(ET::UNKNOWN_TYPE);
	}

	void BytecodeInterpreter::pop()
	{
		if (this->stack.size() > 0)
			this->stack.back().pop_back();
		else
			this->exception(ET::EMPTY_STACK);
	}

	void BytecodeInterpreter::var(std::string const& name)
	{
		this->names.back().push_back(Name(name, Reference(stack.back().size(), stack.size() - 1)));
		stack.back().push_back(TYPE());
	}

	void BytecodeInterpreter::pushref(std::string const& ref)
	{
		this->stack.back().push_back(this->getName(ref).reference());
	}

	void BytecodeInterpreter::pushbackref()
	{
		this->stack.back().push_back(Reference(this->stack.back().size() - 1, this->stack.size() - 1));
	}

	void BytecodeInterpreter::assign()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->stack.at(ref.stackFrame()).at(ref.address());
		TYPE RHS = this->stack.back().back();
		LHS = RHS;
	}

	void BytecodeInterpreter::add()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->stack.at(ref.stackFrame()).at(ref.address());
		TYPE RHS = this->stack.back().back();

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) += std::get<INT>(RHS);
		else if (std::holds_alternative<FLOAT>(LHS) && std::holds_alternative<FLOAT>(RHS))
			std::get<FLOAT>(LHS) += std::get<FLOAT>(RHS);
		else if (std::holds_alternative<FLOAT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<FLOAT>(LHS) += std::get<INT>(RHS);
		else if (std::holds_alternative<INT>(LHS) && std::holds_alternative<FLOAT>(RHS))
			std::get<INT>(LHS) += std::get<FLOAT>(RHS);
	}

	void BytecodeInterpreter::sub()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->stack.at(ref.stackFrame()).at(ref.address());
		TYPE RHS = this->stack.back().back();

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) -= std::get<INT>(RHS);
		else if (std::holds_alternative<FLOAT>(LHS) && std::holds_alternative<FLOAT>(RHS))
			std::get<FLOAT>(LHS) -= std::get<FLOAT>(RHS);
		else if (std::holds_alternative<FLOAT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<FLOAT>(LHS) -= std::get<INT>(RHS);
		else if (std::holds_alternative<INT>(LHS) && std::holds_alternative<FLOAT>(RHS))
			std::get<INT>(LHS) -= std::get<FLOAT>(RHS);
	}

	void BytecodeInterpreter::mul()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->stack.at(ref.stackFrame()).at(ref.address());
		TYPE RHS = this->stack.back().back();

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) *= std::get<INT>(RHS);
		else if (std::holds_alternative<FLOAT>(LHS) && std::holds_alternative<FLOAT>(RHS))
			std::get<FLOAT>(LHS) *= std::get<FLOAT>(RHS);
		else if (std::holds_alternative<FLOAT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<FLOAT>(LHS) *= std::get<INT>(RHS);
		else if (std::holds_alternative<INT>(LHS) && std::holds_alternative<FLOAT>(RHS))
			std::get<INT>(LHS) *= std::get<FLOAT>(RHS);
	}

	void BytecodeInterpreter::div()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->stack.at(ref.stackFrame()).at(ref.address());
		TYPE RHS = this->stack.back().back();

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) /= std::get<INT>(RHS);
		else if (std::holds_alternative<FLOAT>(LHS) && std::holds_alternative<FLOAT>(RHS))
			std::get<FLOAT>(LHS) /= std::get<FLOAT>(RHS);
		else if (std::holds_alternative<FLOAT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<FLOAT>(LHS) /= std::get<INT>(RHS);
		else if (std::holds_alternative<INT>(LHS) && std::holds_alternative<FLOAT>(RHS))
			std::get<INT>(LHS) /= std::get<FLOAT>(RHS);
	}

	void BytecodeInterpreter::mod()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->stack.at(ref.stackFrame()).at(ref.address());
		TYPE RHS = this->stack.back().back();

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) %= std::get<INT>(RHS);
	}

	BytecodeInterpreter::STACK BytecodeInterpreter::getStack()
	{
		return this->stack;
	}

	void BytecodeInterpreter::interpret(Bytecode& bytecode)
	{
		this->names.push_back(std::vector<Name>());
		this->stack.push_back(std::vector<TYPE>());

		for (size_t i = 0; i < bytecode.size(); ++i) {

			if (this->hasCaughtException()) {
				std::cout << "Error: Program exited with code " << static_cast<int>(this->getCaughtException()) << "\n";
				return;
			}

			std::string opcode = bytecode.at(i).getOpcode();
			std::string data = bytecode.at(i).getData();

			if (opcode == "pop") {
				this->pop();
			}
			else if (opcode == "push") {
				this->push(data);
			}
			else if (opcode == "var") {
				this->var(data);
			}
			else if (opcode == "pushref") {
				this->pushref(data);
			}
			else if (opcode == "pushbackref") {
				this->pushbackref();
			}
			else if (opcode == "assign") {
				this->assign();
			}
			else if (opcode == "add") {
				this->add();
			}
		}
	}
}