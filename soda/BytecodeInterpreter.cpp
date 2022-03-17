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
		for (auto n : names.at(1)) { // Checks global variables
			if (n.name() == name)
				return n;
		}
		if (names.size() > 2) { // Checks if program is beyond global scope
			for (auto n : names.back()) { // Checks variables in local scope
				if (n.name() == name)
					return n;
			}
		}
	}

	Class BytecodeInterpreter::getClass(std::string const& name)
	{
		for (auto i : this->cstack) {
			if (i.getName() == name)
				return i;
		}
	}

	BytecodeInterpreter::TYPE& BytecodeInterpreter::getNameValue(std::string const& name)
	{
		Name& n = getName(name);
		return this->stack.at(n.reference().stackFrame()).at(n.reference().address());
	}

	void BytecodeInterpreter::newstack() { this->stack.push_back(std::vector<TYPE>()); }
	void BytecodeInterpreter::popstack() { this->stack.pop_back(); }
	void BytecodeInterpreter::newnames() { this->names.push_back(std::vector<Name>()); }
	void BytecodeInterpreter::popnames() { this->names.pop_back(); }

	void BytecodeInterpreter::newparamstack() { this->params.push_back(std::vector<TYPE>()); }
	void BytecodeInterpreter::popparamstack() { this->params.pop_back(); };

	BytecodeInterpreter::TYPE& BytecodeInterpreter::deref(Reference ref)
	{
		return stack.at(ref.stackFrame()).at(ref.address());
	}

	BytecodeInterpreter::TYPE& BytecodeInterpreter::tracebackReference(TYPE& ref)
	{
		if (std::holds_alternative<Reference>(ref)) {
			Reference r = std::get<Reference>(ref);
			return this->stack.at(r.stackFrame()).at(r.address());
		}
		return ref;
	}

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
		TYPE param = this->tracebackReference(this->stack.back().back());

		this->params.back().push_back(param);
	}

	void BytecodeInterpreter::var(std::string const& name)
	{
		this->names.back().push_back(Name(name, Reference(stack.back().size(), stack.size() - 1)));
		stack.back().push_back(TYPE());
	}

	void BytecodeInterpreter::pushref(std::string const& ref)
	{
		Name n = this->getName(ref);
		TYPE val = this->getNameValue(ref);
		if (std::holds_alternative<Reference>(val)) {
			this->stack.back().push_back(val);
		}
		this->stack.back().push_back(this->getName(ref).reference());
	}

	void BytecodeInterpreter::pushbackref()
	{
		this->stack.back().push_back(Reference(this->stack.back().size() - 1, this->stack.size() - 1));
	}

	void BytecodeInterpreter::assign()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->tracebackReference(this->stack.at(ref.stackFrame()).at(ref.address()));
		TYPE RHS = this->tracebackReference(this->stack.back().back());

		if (std::holds_alternative<Reference>(RHS)) {
			TYPE& RHS = this->stack.back().back();
		}
		LHS = RHS;
	}

	void BytecodeInterpreter::pushname(std::string const& name)
	{
		TYPE& val = this->getNameValue(name);
		this->stack.back().push_back(val);
	}

	void BytecodeInterpreter::add()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->tracebackReference(this->stack.at(ref.stackFrame()).at(ref.address()));
		TYPE RHS = this->tracebackReference(this->stack.back().back());

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
		TYPE& LHS = this->tracebackReference(this->stack.at(ref.stackFrame()).at(ref.address()));
		TYPE RHS = this->tracebackReference(this->stack.back().back());

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
		TYPE& LHS = this->tracebackReference(this->stack.at(ref.stackFrame()).at(ref.address()));
		TYPE RHS = this->tracebackReference(this->stack.back().back());

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
		TYPE& LHS = this->tracebackReference(this->stack.at(ref.stackFrame()).at(ref.address()));
		TYPE RHS = this->tracebackReference(this->stack.back().back());

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
		TYPE& LHS = this->tracebackReference(this->stack.at(ref.stackFrame()).at(ref.address()));
		TYPE RHS = this->tracebackReference(this->stack.back().back());

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
		TYPE& LHS = this->tracebackReference(this->stack.at(ref.stackFrame()).at(ref.address()));
		TYPE RHS = this->tracebackReference(this->stack.back().back());

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) &= std::get<INT>(RHS);
	}

	void BytecodeInterpreter::bitwiseor()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->tracebackReference(this->stack.at(ref.stackFrame()).at(ref.address()));
		TYPE RHS = this->tracebackReference(this->stack.back().back());

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) |= std::get<INT>(RHS);
	}

	void BytecodeInterpreter::bitwisexor()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->tracebackReference(this->stack.at(ref.stackFrame()).at(ref.address()));
		TYPE RHS = this->tracebackReference(this->stack.back().back());

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) ^= std::get<INT>(RHS);
	}

	void BytecodeInterpreter::lshift()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->tracebackReference(this->stack.at(ref.stackFrame()).at(ref.address()));
		TYPE RHS = this->tracebackReference(this->stack.back().back());

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) <<= std::get<INT>(RHS);
	}

	void BytecodeInterpreter::rshift()
	{
		Reference& ref = std::get<Reference>(this->stack.back().at(stack.back().size() - 2));
		TYPE& LHS = this->tracebackReference(this->stack.at(ref.stackFrame()).at(ref.address()));
		TYPE RHS = this->tracebackReference(this->stack.back().back());

		if (std::holds_alternative<INT>(LHS) && std::holds_alternative<INT>(RHS))
			std::get<INT>(LHS) >>= std::get<INT>(RHS);
	}

	void BytecodeInterpreter::pushlistindex()
	{
		size_t index = std::get<INT>(this->stack.back().at(this->stack.back().size() - 1));
		LIST list = std::get<LIST>(this->tracebackReference(this->stack.back().at(this->stack.back().size() - 2)));
		LIST_TYPE t = list.at(index);
		TYPE item;
		stack.back().pop_back();
		if (std::holds_alternative<INT>(t)) {
			item = std::get<INT>(t);
		}
		else if (std::holds_alternative<FLOAT>(t)) {
			item = std::get<FLOAT>(t);
		}
		else if (std::holds_alternative<STRING>(t)) {
			item = std::get<STRING>(t);
		}
		else if (std::holds_alternative<Reference>(t)) {
			std::cout << "AAAAAAAAAAAAAAAAAAAAA";
			Reference r = std::get<Reference>(t);
			item = std::get<LIST>(this->stack.at(r.stackFrame()).at(r.address()));
		}
		this->stack.back().push_back(item);
	}

	void BytecodeInterpreter::pushnewobject(std::string const& className)
	{
		Object obj = Object(this->getClass(className));
		for (auto& i : obj.getMembers()) {
			stack.at(0).push_back(0);
			i.reference().address() = stack.at(0).size() - 1;
		}
		this->stack.back().push_back(obj);
	}

	void BytecodeInterpreter::pushmember(std::string const& name)
	{
		Object& obj = std::get<Object>(this->tracebackReference(this->stack.back().back()));
		for (auto& i : obj.getMembers()) {
			if (i.name() == name) {
				this->stack.back().pop_back();
				this->stack.back().push_back(i.reference());
			}
		}
	}

	BytecodeInterpreter::STACK BytecodeInterpreter::getStack()
	{
		return this->stack;
	}

	void BytecodeInterpreter::interpret(Bytecode& bytecode)
	{
		this->names.push_back(std::vector<Name>()); // "heap" memory
		this->stack.push_back(std::vector<TYPE>());

		this->names.push_back(std::vector<Name>()); // global stack scope
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
			else if (opcode == "class") {
				cstack.push_back(Class(data));
			}
			else if (opcode == "member") {
				cstack.back().addMember(Name(data, Reference(0, 0)));
			}
			else if (opcode == "return") {
				this->RETURN = stack.back().back();

				i = js.back();
				js.pop_back();
				continue;
			}
			else if (opcode == "varparam") {
				this->var(data);
				this->stack.back().at(this->getName(data).reference().address()) = this->params.back().front();
				std::reverse(params.back().begin(), params.back().end());
				params.back().pop_back();
				std::reverse(params.back().begin(), params.back().end());
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
				if (this->call(data, params.back(), stack)) {
					this->RETURN = this->getReturnValue();
				}
				else {
					js.push_back(i + 1);
					i = std::get<INT>(this->stack.at(1).at(this->getName(data).reference().address())) - 1;
					continue;
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
			else if (opcode == "pushlistindex") {
				this->pushlistindex();
			}
			else if (opcode == "pushnewobject") {
				this->pushnewobject(data);
			}
			else if (opcode == "pushmember") {
				this->pushmember(data);
			}
		}
	}
}