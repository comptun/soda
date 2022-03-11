#include "Soda.h"

namespace sda
{
	int Soda::main()
	{
		lex("main.soda");
		translate(getTokens());
		//std::cout << getBytecode();
		interpret(getBytecode());

		std::cin.get();
		return 1;
	}
}