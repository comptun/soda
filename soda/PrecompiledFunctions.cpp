#include "PrecompiledFunctions.h"

#include <iostream>

namespace sda
{
	void PrecompiledFunctions::putchar(INT out)
	{
		std::cout << static_cast<char>(out);
	}

	void PrecompiledFunctions::putstr(STRING out)
	{
		std::cout << out;
	}

	void PrecompiledFunctions::pause()
	{
		std::cin.get();
	}

	void PrecompiledFunctions::print(PARAMS& params)
	{
		for (auto& p : params)
		{
			if (std::holds_alternative<INT>(p))
				std::cout << std::get<INT>(p);
			else if (std::holds_alternative<FLOAT>(p))
				std::cout << std::get<FLOAT>(p);
			else if (std::holds_alternative<STRING>(p))
				std::cout << std::get<STRING>(p);
		}
	}

	void PrecompiledFunctions::printType(TYPE& p)
	{
		if (std::holds_alternative<INT>(p))
			std::cout << std::get<INT>(p);
		else if (std::holds_alternative<FLOAT>(p))
			std::cout << std::get<FLOAT>(p);
		else if (std::holds_alternative<STRING>(p))
			std::cout << std::get<STRING>(p);
	}

	void PrecompiledFunctions::printvalist(STACK& stack, LIST& valist)
	{
		for (auto& r : valist) {
			TYPE type = stack.at(r.stackFrame()).at(r.address());
			this->printType(type);
		}
	}

	void PrecompiledFunctions::input(PARAMS& params)
	{
		this->print(params);
		std::string out;
		std::cin >> out;
		RETURN = out;
	}

	void PrecompiledFunctions::endl()
	{
		RETURN = "\n";
	}

	void PrecompiledFunctions::list(PARAMS& params, STACK& stack)
	{
		LIST l;
		for (auto& t : params) {
			stack.front().push_back(t);
			l.push_back(Reference(stack.front().size() - 1, 0));
		}
		RETURN = l;
	}

	void PrecompiledFunctions::rand(INT lowerBound, INT upperBound)
	{
		RETURN = (std::rand() % (upperBound - lowerBound)) + lowerBound;
	}

	void PrecompiledFunctions::disassemble(STRING str)
	{
		Lexer l = Lexer();
		l.lexString(str);
		TokenList tokens = l.getTokens();
		Translator t = Translator();
		t.translate(tokens);
		std::cout << t.getBytecode();
	}

	void PrecompiledFunctions::bytecode(LIST bc)
	{

	}

	void PrecompiledFunctions::push(STACK& stack, LIST list, TYPE item)
	{
		stack.at(0).push_back(item);
		list.push_back(Reference(stack.at(0).size() - 1, 0));
		RETURN = list;
	}
	void PrecompiledFunctions::pop(LIST list)
	{
		list.pop_back();
		RETURN = list;
	}

	PrecompiledFunctions::TYPE PrecompiledFunctions::getReturnValue()
	{
		return this->RETURN;
	}

	bool PrecompiledFunctions::call(std::string const& name, PARAMS& params, STACK& stack)
	{
		RETURN = 0;
		if (name == "putchar") {
			this->putchar(std::get<INT>(params.front()));
			return true;
		}
		else if (name == "putstr") {
			this->putstr(std::get<STRING>(params.front()));
			return true;
		}
		else if (name == "pause") {
			this->pause();
			return true;
		}
		else if (name == "print") {
			this->print(params);
			return true;
		}
		else if (name == "printvalist") {
			this->printvalist(stack, std::get<LIST>(params.front()));
			return true;
		}
		else if (name == "input") {
			this->input(params);
			return true;
		}
		else if (name == "endl") {
			this->endl();
			return true;
		}
		else if (name == "list") {
			this->list(params, stack);
			return true;
		}
		else if (name == "rand") {
			this->rand(std::get<INT>(params.at(0)), std::get<INT>(params.at(1)));
			return true;
		}
		else if (name == "concat") {
			RETURN = params.front();
			return true;
		}
		else if (name == "disassemble") {
			this->disassemble(std::get<STRING>(params.at(0)));
			return true;
		}
		else if (name == "execute") {
			//this->execute(std::get<LIST>(params.at(0)));
		}
		else if (name == "push") {
			this->push(stack, std::get<LIST>(params.at(0)), params.at(1));
			return true;
		}
		else if (name == "pop") {
			this->pop(std::get<LIST>(params.at(0)));
			return true;
		}

		return false;
	}
}