#include "Soda.h"

namespace sda
{
	int Soda::main()
	{
		lex("main.soda");
		translate(getTokens());
		interpret(getBytecode());
		//std::cout << getBytecode();
		std::cin.get();
		return 1;
	}
}