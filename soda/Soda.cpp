#include "Soda.h"
#include "Translator.h"

namespace sda
{
	int Soda::main()
	{
		lex("main.soda");
		translate(getTokens());
		std::cout << getBytecode();
		return 1;
	}
}