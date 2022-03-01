#ifndef TOKEN_TYPES_H
#define TOKEN_TYPES_H

namespace sda
{
	enum class TT
	{
		EMPTY = 0,
		FUNCTION,
		ASSIGNMENT,
		SEMICOLON,
		LBRACKET,
		RBRACKET,
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE,
		MODULUS,
		LCURLYBRACE,
		RCURLYBRACE,
		DOT,
		COMMA,
		PLUSEQUALS,
		SUBTRACTEQUALS,
		MULTIPLYEQUALS,
		DIVIDEEQUALS,
		MODULUSEQUALS,

		NAME,
	};
}

#endif
