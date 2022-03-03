#ifndef SODA_H
#define SODA_H

#include "Lexer.h"
#include "Translator.h"

namespace sda
{
	class Soda : public Lexer, public Translator
	{
	public:
		int main();
	};
}

#endif