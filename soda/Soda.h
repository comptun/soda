#ifndef SODA_H
#define SODA_H

#include "Lexer.h"
#include "Translator.h"
#include "BytecodeInterpreter.h"

namespace sda
{
	class Soda : public Lexer, 
				 public Translator, 
				 public BytecodeInterpreter
	{
	public:
		int main();
	};
}

#endif