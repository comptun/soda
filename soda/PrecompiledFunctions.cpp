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

	void PrecompiledFunctions::print(PARAMS& stack)
	{
		for (auto& p : stack)
		{
			if (std::holds_alternative<INT>(p))
				std::cout << std::get<INT>(p);
			else if (std::holds_alternative<FLOAT>(p))
				std::cout << std::get<FLOAT>(p);
			else if (std::holds_alternative<STRING>(p))
				std::cout << std::get<STRING>(p);
		}
	}

	void PrecompiledFunctions::input(PARAMS& stack)
	{
		this->print(stack);
		std::string out;
		std::cin >> out;
		RETURN = out;
	}

	void PrecompiledFunctions::endl()
	{
		RETURN = "\n";
	}

	PrecompiledFunctions::TYPE PrecompiledFunctions::getReturnValue()
	{
		return this->RETURN;
	}

	bool PrecompiledFunctions::call(std::string const& name, PARAMS& stack)
	{
		RETURN = 0;
		if (name == "putchar") {
			this->putchar(std::get<INT>(stack.front()));
			return true;
		}
		else if (name == "putstr") {
			this->putstr(std::get<STRING>(stack.front()));
			return true;
		}
		else if (name == "pause") {
			this->pause();
			return true;
		}
		else if (name == "print") {
			this->print(stack);
			return true;
		}
		else if (name == "input") {
			this->input(stack);
			return true;
		}
		else if (name == "endl") {
			this->endl();
			return true;
		}

		return false;
	}
}