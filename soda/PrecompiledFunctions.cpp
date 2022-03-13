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
			if (std::holds_alternative<INT>(t))
				l.push_back(std::get<INT>(t));
			else if (std::holds_alternative<FLOAT>(t))
				l.push_back(std::get<FLOAT>(t));
			else if (std::holds_alternative<STRING>(t))
				l.push_back(std::get<STRING>(t));
			else if (std::holds_alternative<Reference>(t))
				l.push_back(std::get<Reference>(t));
			else if (std::holds_alternative<LIST>(t)) {
				stack.back().push_back(t);
				l.push_back(Reference(stack.back().size() - 1, stack.size() - 1));
			}
		}
		RETURN = l;
	}

	void PrecompiledFunctions::rand(INT lowerBound, INT upperBound)
	{
		RETURN = (std::rand() % (upperBound - lowerBound)) + lowerBound;
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

		return false;
	}
}