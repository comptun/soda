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

	void BytecodeInterpreter::newstack() { this->stack.push_back(std::vector<TYPE>()); }
	void BytecodeInterpreter::popstack() { this->stack.pop_back(); }
	void BytecodeInterpreter::newnames() { this->names.push_back(std::vector<Name>()); }
	void BytecodeInterpreter::popnames() { this->names.pop_back(); }

	void BytecodeInterpreter::newparamstack() { this->params.push_back(std::vector<TYPE>()); }
	void BytecodeInterpreter::popparamstack() { this->params.pop_back(); };

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

	void BytecodeInterpreter::pushparam()
	{
		this->params.back().push_back(stack.back().back());
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

	void BytecodeInterpreter::pushname(std::string const& name)
	{
		this->stack.back().push_back(this->stack.at(this->getName(name).reference().stackFrame()).at(this->getName(name).reference().address()));
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

		if (std::holds_alternative<INT>(RHS))
			if (std::get<INT>(RHS) == 0) {
				this->exception(ET::DIVISION_BY_ZERO);
				return;
			}

		else if (std::holds_alternative<FLOAT>(RHS))
			if (std::get<FLOAT>(RHS) == 0.0) {
				this->exception(ET::DIVISION_BY_ZERO);
				return;
			}

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

		if (std::holds_alternative<INT>(RHS))
			if (std::get<INT>(RHS) == 0) {
				this->exception(ET::DIVISION_BY_ZERO);
				return;
			}

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) %= std::get<INT>(RHS);
	}

	void BytecodeInterpreter::bitwiseand()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->stack.at(ref.stackFrame()).at(ref.address());
		TYPE RHS = this->stack.back().back();

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) &= std::get<INT>(RHS);
	}

	void BytecodeInterpreter::bitwiseor()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->stack.at(ref.stackFrame()).at(ref.address());
		TYPE RHS = this->stack.back().back();

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) |= std::get<INT>(RHS);
	}

	void BytecodeInterpreter::bitwisexor()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->stack.at(ref.stackFrame()).at(ref.address());
		TYPE RHS = this->stack.back().back();

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) ^= std::get<INT>(RHS);
	}

	void BytecodeInterpreter::lshift()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->stack.at(ref.stackFrame()).at(ref.address());
		TYPE RHS = this->stack.back().back();

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) <<= std::get<INT>(RHS);
	}

	void BytecodeInterpreter::rshift()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->stack.at(ref.stackFrame()).at(ref.address());
		TYPE RHS = this->stack.back().back();

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) >>= std::get<INT>(RHS);
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
			else if (opcode == "pushparam") {
				this->pushparam();
			}
			else if (opcode == "var") {
				this->var(data);
			}
			else if (opcode == "function") {
				this->var(data);
				this->pushref(data);
				this->stack.back().push_back(static_cast<INT>(i) + 1);
				this->assign();
				this->pop();
				this->pop();
				for (; i < bytecode.size() && bytecode.at(i).getOpcode() != "endfunction"; ++i);
			}
			else if (opcode == "return") {
				this->RETURN = stack.back().back();
			}
			else if (opcode == "varparam") {
				this->var(data);
				this->stack.back().at(this->getName(data).reference().address()) = this->params.back().front();
				this->params.back().erase(this->params.back().begin());
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
			else if (opcode == "pushname") {
				this->pushname(data);
			}
			else if (opcode == "add") {
				this->add();
			}
			else if (opcode == "sub") {
				this->sub();
			}
			else if (opcode == "mul") {
				this->mul();
			}
			else if (opcode == "div") {
				this->div();
			}
			else if (opcode == "and") {
				this->bitwiseand();
			}
			else if (opcode == "or") {
				this->bitwiseor();
			}
			else if (opcode == "xor") {
				this->bitwisexor();
			}
			else if (opcode == "lshift") {
				this->lshift();
			}
			else if (opcode == "rshift") {
				this->rshift();
			}
			else if (opcode == "newparamstack") {
				this->newparamstack();
			}
			else if (opcode == "popparamstack") {
				this->popparamstack();
			}
			else if (opcode == "call") {
				if (this->call(data, params.back())) {
					this->RETURN = this->getReturnValue();
				}
				
			}
			else if (opcode == "newstack") {
				this->newstack();
				this->newnames();
			}
			else if (opcode == "popstack") {
				this->popstack();
				this->popnames();
			}
			else if (opcode == "pushreturnvalue") {
				this->stack.back().push_back(RETURN);
			}
		}
	}
}