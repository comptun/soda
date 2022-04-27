#include "Soda.h"

namespace sda
{
	int Soda::main(const std::string& filePath)
	{
		srand((unsigned)time(0));

		lex(filePath);
		translate(getTokens());
		//std::cout << getBytecode();
		interpret(getBytecode());
		//std::cin.get();
		return 1;
	}
}