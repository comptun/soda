#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <string>
#include "Token.h"
#include "Byte.h"

namespace sda
{
	class Instruction
	{
	private:
		std::string instruction;
		std::string data;
	public:
		void setInstruction(std::string const& instruction);
		void setData(std::string const& data);
		std::string& getInstruction();
		std::string& getData();
		Instruction(std::string const& opcode, std::string const& data);
		Instruction();
	};

	class InstructionStack
	{
	private:
		std::vector<Instruction> istack;
	public:
		friend InstructionStack& operator<<(InstructionStack& out, Instruction i);
		void push(Instruction instruction);
		void push(std::string instruction, std::string data);
		void pop();
		Instruction& at(size_t index);
		size_t size();
		Instruction& back();
	};

	class Translator
	{
	private:
		Bytecode bytecode;
		InstructionStack istack;
		bool isOperator(TT type);
	public:
		void translate(TokenList& tokens);
		Bytecode& getBytecode();
	};
}

#endif