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

	bool PrecompiledFunctions::call(std::string const& name, PARAMS& stack)
	{
		if (name == "putchar") {
			this->putchar(std::get<INT>(stack.front()));
			return true;
		}
		else if (name == "putstr") {
			this->putstr(std::get<STRING>(stack.front()));
			return true;
		}

		return false;
	}
}