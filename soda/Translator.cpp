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
		istack << Instruction("start", "EMPTY");
		for (size_t i = 0; i < tokens.size();) {
			if (tokens.at(i).getType() == TT::SEMICOLON) {
				if (istack.back().getInstruction() == "assignment") {
					bytecode << Byte("pop", "0");
					bytecode << Byte("assign", "0");
					bytecode << Byte("pop", "0");
					bytecode << Byte("pop", "0");
					istack.pop();
				}
				
				i += 1;
			}
			else if (tokens.at(i).getType() == TT::VAR) {
				bytecode << Byte("var", tokens.at(i + 1).getName());
				i += 1;
			}
			else if (tokens.at(i).getType() == TT::NAME) {
				if (tokens.at(i + 1).getType() == TT::ASSIGNMENT) {
					bytecode << Byte("pushref", tokens.at(i).getName());
				}
				else if (istack.back().getInstruction() == "operator") {
					bytecode << Byte("pushname", tokens.at(i).getName());
					bytecode << Byte(istack.back().getData(), "0");
					bytecode << Byte("pop", "0");
					istack.pop();
				}
				else {
					bytecode << Byte("pushname", tokens.at(i).getName());
					bytecode << Byte("pushbackref", "0");
				}
				i += 1;
			}
			else if (tokens.at(i).getType() == TT::ASSIGNMENT) {
				istack << Instruction("assignment", "EMPTY");
				i += 1;
			}
			else if (tokens.at(i).getType() == TT::NUM) {
				if (istack.back().getInstruction() == "operator") {
					bytecode << Byte("push", tokens.at(i).getName());
					bytecode << Byte(istack.back().getData(), "0");
					bytecode << Byte("pop", "0");
					istack.pop();
				}
				else {
					bytecode << Byte("push", tokens.at(i).getName());
					bytecode << Byte("pushbackref", "0");
				}
				i += 1;
			}
			else if (tokens.at(i).getType() == TT::ADD ||
				tokens.at(i).getType() == TT::SUBTRACT ||
				tokens.at(i).getType() == TT::MULTIPLY ||
				tokens.at(i).getType() == TT::DIVIDE) {
				TT op = tokens.at(i).getType();
				if (op == TT::ADD)
					istack << Instruction("operator", "add");
				else if (op == TT::SUBTRACT)
					istack << Instruction("operator", "sub");
				else if (op == TT::MULTIPLY)
					istack << Instruction("operator", "mul");
				else if (op == TT::DIVIDE)
					istack << Instruction("operator", "div");
				i += 1;
			}
		}
	}
}