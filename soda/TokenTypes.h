#ifndef TOKEN_TYPES_H
#define TOKEN_TYPES_H

namespace sda
{
	enum class TT
	{
		EMPTY = 0,
		FUNCTION,
		VAR,
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
		LSQUAREBRACE,
		RSQUAREBRACE,
		DOT,
		COMMA,
		PLUSEQUALS,
		SUBTRACTEQUALS,
		MULTIPLYEQUALS,
		DIVIDEEQUALS,
		MODULUSEQUALS,
		DOUBLECOLON,
		EQUALTO,
		LESSTHAN,
		GREATERTHAN,
		GREATERTHANEQUALTO,
		LESSTHANEQUALTO,
		NOTEQUALTO,
		AND,
		OR,
		NOT,
		BITWISEAND,
		BITWISEOR,
		BITWISEXOR,
		LEFTSHIFT,
		RIGHTSHIFT,

		STRING,
		NAME,
		NUM,
	};
}

#endif
