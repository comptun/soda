#include "Soda.h"

namespace sda
{
	int Soda::main()
	{
		lex("main.soda");
		translate(getTokens());
		std::cout << getBytecode();
		interpret(getBytecode());
		std::cout << "\n\n\n" << std::get<long long int>(getStack().back().front());
		std::cin.get();
		return 1;
	}
}