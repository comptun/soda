#include "Translator.h"
#include "Byte.h"
#include "TokenTypes.h"

#include <iostream>

namespace sda
{
	void Instruction::setInstruction(std::string const& instruction)
	{
		this->instruction = instruction;
	}
	void Instruction::setData(std::string const& data)
	{
		this->data = data;
	}
	std::string& Instruction::getInstruction()
	{
		return this->instruction;
	}
	std::string& Instruction::getData()
	{
		return this->data;
	}
	Instruction::Instruction(std::string const& instruction, std::string const& data)
	{
		this->instruction = instruction;
		this->data = data;
	}
	Instruction::Instruction()
	{
		this->instruction = "EMPTY";
		this->data = "EMPTY";
	}

	InstructionStack& operator<<(InstructionStack& out, Instruction i)
	{
		out.push(i);
		return out;
	}
	void InstructionStack::push(Instruction instruction)
	{
		this->istack.push_back(instruction);
	}
	void InstructionStack::push(std::string instruction, std::string data)
	{
		this->istack.push_back(Instruction(instruction, data));
	}
	void InstructionStack::pop()
	{
		this->istack.pop_back();
	}
	Instruction& InstructionStack::at(size_t index)
	{
		return this->istack.at(index);
	}
	size_t InstructionStack::size()
	{
		return this->istack.size();
	}
	Instruction& InstructionStack::back()
	{
		return this->istack.back();
	}

	Bytecode& Translator::getBytecode()
	{
		return this->bytecode;
	}

	void Translator::translate(TokenList& tokens)
	{
		for (size_t i = 0; i < tokens.size(); ++i) {
			if (tokens.at(i).getType() == TT::SEMICOLON) {
				if (istack.back().getInstruction() == "assignment") {
					bytecode << Byte("pop", "0");
					bytecode << Byte("assign", "0");
					bytecode << Byte("pop", "0");
					bytecode << Byte("pop", "0");
				}
				istack.pop();
			}
			else if (tokens.at(i).getType() == TT::VAR) {
				istack << Instruction("assignment", tokens.at(i + 1).getName());
				bytecode << Byte("var", tokens.at(i + 1).getName());
				bytecode << Byte("loadref", tokens.at(i + 1).getName());
				bytecode << Byte("load", "0");
				bytecode << Byte("loadbackref", "0");
			}
		}
	}
}