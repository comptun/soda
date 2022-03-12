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

	bool Translator::isOperator(TT type)
	{
		return type == TT::ADD ||
			type == TT::SUBTRACT ||
			type == TT::MULTIPLY ||
			type == TT::DIVIDE ||
			type == TT::BITWISEAND ||
			type == TT::BITWISEOR ||
			type == TT::BITWISEXOR ||
			type == TT::LEFTSHIFT ||
			type == TT::RIGHTSHIFT;
	}

	void Translator::translate(TokenList& tokens)
	{
		istack << Instruction("start", "EMPTY");
		for (size_t i = 0; i < tokens.size();) {

			TT type = tokens.at(i).getType();
			std::string name = tokens.at(i).getName();

			if (type == TT::SEMICOLON) {
				if (istack.back().getInstruction() == "assignment") {
					bytecode << Byte("pop", "0");
					bytecode << Byte("assign", "0");
					bytecode << Byte("pop", "0");
					bytecode << Byte("pop", "0");
					istack.pop();
				}
				else if (istack.back().getInstruction() == "return") {
					bytecode << Byte("pop", "0");
					bytecode << Byte("return", "0");
					istack.pop();
				}
				i += 1;
			}
			else if (type == TT::VAR) {
				bytecode << Byte("var", tokens.at(i + 1).getName());
				if (tokens.at(i + 2).getType() == TT::SEMICOLON) {
					i += 3;
				}
				else {
					i += 1;
				}
			}
			else if (type == TT::NAME) {
				if (istack.back().getInstruction() == "functionparams") {
					if (tokens.at(i + 1).getType() == TT::LBRACKET) {
						bytecode << Byte("function", name);
						i += 2;
						continue;
					}
					else if (tokens.at(i + 1).getType() == TT::COMMA || tokens.at(i + 1).getType() == TT::RBRACKET) {
						bytecode << Byte("varparam", name);
						if (tokens.at(i + 1).getType() == TT::COMMA)
							i += 2;
						else
							i += 1;
						continue;
					}
				}
				else if (tokens.at(i + 1).getType() == TT::LBRACKET) {
					bytecode << Byte("newparamstack", "0");
					istack << Instruction("functioncall", tokens.at(i).getName());
					if (tokens.at(i + 2).getType() == TT::RBRACKET) {
						bytecode << Byte("newstack", "0");
						bytecode << Byte("call", name);
						bytecode << Byte("popparamstack", "0");
						bytecode << Byte("popstack", "0");
						istack.pop();
						if (istack.back().getInstruction() == "operator") {
							bytecode << Byte("pushreturnvalue", "0");
							bytecode << Byte(istack.back().getData(), "0");
							bytecode << Byte("pop", "0");
						}
						else {
							bytecode << Byte("pushreturnvalue", "0");
							bytecode << Byte("pushbackref", "0");
						}
						i += 3;
						continue;
					}
					i += 2;
					continue;
				}
				else if (tokens.at(i + 1).getType() == TT::ASSIGNMENT) {
					bytecode << Byte("pushref", name);
				}
				else if (istack.back().getInstruction() == "operator") {
					bytecode << Byte("pushname", name);
					bytecode << Byte(istack.back().getData(), "0");
					bytecode << Byte("pop", "0");
					istack.pop();
				}
				else {
					bytecode << Byte("pushname", name);
					bytecode << Byte("pushbackref", "0");
				}
				i += 1;
			}
			else if (type == TT::ASSIGNMENT) {
				istack << Instruction("assignment", "EMPTY");
				i += 1;
			}
			else if (type == TT::NUM || type == TT::STRING) {
				if (istack.back().getInstruction() == "operator") {
					bytecode << Byte("push", name);
					bytecode << Byte(istack.back().getData(), "0");
					bytecode << Byte("pop", "0");
					istack.pop();
				}
				else {
					bytecode << Byte("push", name);
					bytecode << Byte("pushbackref", "0");
				}
				i += 1;
			}
			else if (isOperator(type)) {
				if (type == TT::ADD)
					istack << Instruction("operator", "add");
				else if (type == TT::SUBTRACT)
					istack << Instruction("operator", "sub");
				else if (type == TT::MULTIPLY)
					istack << Instruction("operator", "mul");
				else if (type == TT::DIVIDE)
					istack << Instruction("operator", "div");
				else if (type == TT::BITWISEAND)
					istack << Instruction("operator", "and");
				else if (type == TT::BITWISEOR)
					istack << Instruction("operator", "or");
				else if (type == TT::BITWISEXOR)
					istack << Instruction("operator", "xor");
				else if (type == TT::LEFTSHIFT)
					istack << Instruction("operator", "lshift");
				else if (type == TT::RIGHTSHIFT)
					istack << Instruction("operator", "rshift");
				i += 1;
			}
			else if (type == TT::COMMA) {
				bytecode << Byte("pop", "0");
				bytecode << Byte("pushparam", "0");
				bytecode << Byte("pop", "0");
				i += 1;
			}
			else if (type == TT::RBRACKET) {
				if (istack.back().getInstruction() == "functionparams") {
					if (tokens.at(i + 1).getType() == TT::LCURLYBRACE) {
						std::string funcname = istack.back().getData();
						istack.pop();
						istack << Instruction("functiondef", funcname);
						i += 2;
						continue;
					}
				}
				else if (istack.back().getInstruction() == "functioncall") {
					bytecode << Byte("pop", "0");
					bytecode << Byte("pushparam", "0");
					bytecode << Byte("pop", "0");
					bytecode << Byte("newstack", "0");
					bytecode << Byte("call", istack.back().getData());
					bytecode << Byte("popparamstack", "0");
					bytecode << Byte("popstack", "0");
					istack.pop();
					if (istack.back().getInstruction() == "operator") {
						bytecode << Byte("pushreturnvalue", "0");
						bytecode << Byte(istack.back().getData(), "0");
						bytecode << Byte("pop", "0");
					}
					else {
						bytecode << Byte("pushreturnvalue", "0");
						bytecode << Byte("pushbackref", "0");
					}
				}
				i += 1;
			}
			else if (type == TT::FUNCTION) {
				istack << Instruction("functionparams", tokens.at(i + 1).getName());
				i += 1;
			}
			else if (type == TT::RCURLYBRACE) {
				bytecode << Byte("push", "0");
				bytecode << Byte("return", "0");
				bytecode << Byte("endfunction", istack.back().getData());
				istack.pop();
				i += 1;
			}
			else if (type == TT::RETURN) {
				istack << Instruction("return", "0");
				i += 1;
			}
		}
	}
}