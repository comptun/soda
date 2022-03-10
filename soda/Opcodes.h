#ifndef OPCODES_H
#define OPCODES_H

namespace sda
{
	enum class OP
	{
		PUSH = 0,
		PUSHREF,
		PUSHBACKREF,
		VAR,
		ASSIGN,
		ADD,
		SUM,
		MUL,
		DIV,
		AND,
		OR,
		XOR,
		LSHIFT,
		RSHIFT,
	};
}

#endif
