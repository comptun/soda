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
			type == TT::RIGHTSHIFT ||
			type == TT::DOUBLESTAR;
	}

	void Translator::optimise()
	{
		Bytecode newBytecode;
		for (size_t i = 0; i < this->bytecode.size();) {
			if (bytecode.at(i).getOpcode() == "pushbackref" && i < this->bytecode.size() - 1) {
				if (bytecode.at(i + 1).getOpcode() == "pop") {
					i += 2;
					continue;
				}
				
			}
			if (bytecode.at(i).getOpcode() == "push" && i < this->bytecode.size() - 1) {
				if (bytecode.at(i + 1).getOpcode() == "return" && bytecode.at(i - 1).getOpcode() == "return") {
					i += 2;
					continue;
				}

			}
			newBytecode.push_byte(this->bytecode.at(i));
			i += 1;
		}
		this->bytecode = newBytecode;
	}

	void Translator::translate(TokenList& tokens)
	{
		istack << Instruction("start", "EMPTY");
		for (size_t i = 0; i < tokens.size();) {

			TT type = tokens.at(i).getType();
			std::string name = tokens.at(i).getName();

			if (name == "class") {
				bytecode << Byte("class", tokens.at(i + 1).getName());
				if (tokens.at(i + 2).getType() == TT::LCURLYBRACE) {
					istack << Instruction("classdef", tokens.at(i + 1).getName());
				}
				i += 3;
			}
			else if (name == "member") {
				bytecode << Byte("member", tokens.at(i + 1).getName());
				i += 3;
			}
			else if (name == "new") {
				bytecode << Byte("newparamstack");
				bytecode << Byte("pushnewobject", tokens.at(i + 1).getName());
				bytecode << Byte("pushparam");
				bytecode << Byte("pop");
				if (tokens.at(i + 3).getType() == TT::RBRACKET) {
					bytecode << Byte("newstack");
					bytecode << Byte("call", tokens.at(i + 1).getName());
					bytecode << Byte("popparamstack");
					bytecode << Byte("popstack");
					bytecode << Byte("pushreturnvalue");
					bytecode << Byte("pushbackref");
					i += 4;
				}
				else {
					istack << Instruction("functioncall", tokens.at(i + 1).getName());
					i += 3;
				}
			}
			else if (type == TT::DOT) {
				if (tokens.at(i + 2).getType() == TT::LBRACKET) {
					bytecode << Byte("pop");
					bytecode << Byte("newparamstack");
					bytecode << Byte("pushparam");
					bytecode << Byte("pop");
					if (tokens.at(i + 3).getType() == TT::RBRACKET) {
						bytecode << Byte("newstack");
						bytecode << Byte("call", tokens.at(i + 1).getName());
						bytecode << Byte("popparamstack");
						bytecode << Byte("popstack");
						bytecode << Byte("pushreturnvalue");
						bytecode << Byte("pushbackref");
						i += 4;
					}
					else {
						istack << Instruction("functioncall", tokens.at(i + 1).getName());
						i += 3;
					}
				}
				else {
					bytecode << Byte("pop");
					bytecode << Byte("pushmember", tokens.at(i + 1).getName());
					bytecode << Byte("pushbackref");
					i += 2;
				}
			}
			else if (type == TT::LSQUAREBRACE) {
				bytecode << Byte("pop");
				istack << Instruction("listindex", "0");
				i += 1;
			}
			else if (type == TT::RSQUAREBRACE) {
				if (istack.back().getInstruction() == "operator") {
					bytecode << Byte("pop");
					bytecode << Byte(istack.back().getData());
					bytecode << Byte("pop");
					istack.pop();
				}
				if (istack.back().getInstruction() == "listindex") {
					bytecode << Byte("pop");
					bytecode << Byte("pushlistindex");
					bytecode << Byte("pushbackref");
					istack.pop();
					i += 1;
				}
			}
			else if (type == TT::SEMICOLON) {
				if (istack.back().getInstruction() == "operator") {
					bytecode << Byte("pop");
					bytecode << Byte(istack.back().getData());
					bytecode << Byte("pop");
					istack.pop();
				}
				if (istack.back().getInstruction() == "assignment") {
					bytecode << Byte("pop");
					bytecode << Byte("assign");
					bytecode << Byte("pop");
					bytecode << Byte("pop");
					istack.pop();
				}
				else if (istack.back().getInstruction() == "return") {
					bytecode << Byte("pop");
					bytecode << Byte("return");
					istack.pop();
				}
				else if (istack.back().getInstruction() == "returnref") {
					bytecode << Byte("pop");
					bytecode << Byte("returnref");
					istack.pop();
				}
				i += 1;
			}
			else if (type == TT::VAR) {
				if (istack.back().getInstruction() == "classdef") {
					bytecode << Byte("member", tokens.at(i + 1).getName());
					i += 3;
					continue;
				}
				bytecode << Byte("var", tokens.at(i + 1).getName());
				if (tokens.at(i + 2).getType() == TT::SEMICOLON) {
					i += 3;
				}
				else {
					i += 1;
				}
			}
			else if (type == TT::NAME) {
				if (istack.back().getInstruction() == "classdef") {
					if (name == istack.back().getData()) {
						bytecode << Byte("function", name);
						bytecode << Byte("varparam", "this");
						istack << Instruction("constructor", name);
						istack << Instruction("functionparams", name);
						i += 2;
						continue;
					}
				}
				else if (istack.back().getInstruction() == "functionparams") {
					if (tokens.at(i + 1).getType() == TT::LBRACKET) {
						bytecode << Byte("function", name);
						if (istack.at(istack.size() - 2).getInstruction() == "classdef") {
							bytecode << Byte("varparam", "this");
						}
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
					bytecode << Byte("newparamstack");
					istack << Instruction("functioncall", tokens.at(i).getName());
					if (tokens.at(i + 2).getType() == TT::RBRACKET) {
						bytecode << Byte("newstack");
						bytecode << Byte("call", name);
						bytecode << Byte("popparamstack");
						bytecode << Byte("popstack");
						istack.pop();
						if (istack.back().getInstruction() == "operator") {
							bytecode << Byte("pushreturnvalue");
							bytecode << Byte(istack.back().getData());
							bytecode << Byte("pop");
							istack.pop();
						}
						else {
							bytecode << Byte("pushreturnvalue");
							bytecode << Byte("pushbackref");
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
				else {
					//bytecode << Byte("pushname", name);
					bytecode << Byte("pushref", name);
					bytecode << Byte("pushbackref");
				}
				i += 1;
			}
			else if (type == TT::ASSIGNMENT) {
				istack << Instruction("assignment", "EMPTY");
				i += 1;
			}
			else if (type == TT::NUM || type == TT::STRING) {
				bytecode << Byte("push", name);
				bytecode << Byte("pushbackref");
				i += 1;
			}
			else if (isOperator(type)) {
				if (istack.back().getInstruction() == "operator") {
					bytecode << Byte("pop");
					bytecode << Byte(istack.back().getData());
					bytecode << Byte("pop");
					istack.pop();
				}
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
				else if (type == TT::DOUBLESTAR)
					istack << Instruction("operator", "pow");
				i += 1;
			}
			else if (type == TT::COMMA) {
				if (istack.back().getInstruction() == "operator") {
					bytecode << Byte("pop");
					bytecode << Byte(istack.back().getData());
					bytecode << Byte("pop");
					istack.pop();
				}
				if (istack.back().getInstruction() == "assignment") {
					bytecode << Byte("pop");
					bytecode << Byte("assign");
					bytecode << Byte("pop");
					bytecode << Byte("pop");
				}
				if (istack.back().getInstruction() == "functioncall") {
					bytecode << Byte("pop");
					bytecode << Byte("pushparam");
					bytecode << Byte("pop");
				}
				i += 1;
			}
			else if (type == TT::RBRACKET) {
				if (istack.back().getInstruction() == "operator") {
					bytecode << Byte("pop");
					bytecode << Byte(istack.back().getData());
					bytecode << Byte("pop");
					istack.pop();
				}
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
					bytecode << Byte("pop");
					bytecode << Byte("pushparam");
					bytecode << Byte("pop");
					bytecode << Byte("newstack");
					bytecode << Byte("call", istack.back().getData());
					bytecode << Byte("popparamstack");
					bytecode << Byte("popstack");
					istack.pop();
					if (istack.back().getInstruction() == "operator") {
						bytecode << Byte("pushreturnvalue");
						bytecode << Byte("pushbackref");
					}
					else {
						bytecode << Byte("pushreturnvalue");
						bytecode << Byte("pushbackref");
					}
				}
				i += 1;
			}
			else if (type == TT::FUNCTION) {
				istack << Instruction("functionparams", tokens.at(i + 1).getName());
				i += 1;
			}
			else if (type == TT::RCURLYBRACE) {
				if (istack.at(istack.size() - 2).getInstruction() == "constructor") {
					//bytecode << Byte("pushname", "this");
					bytecode << Byte("pushref", "this");
					bytecode << Byte("return");
					bytecode << Byte("endfunction", istack.back().getData());
					istack.pop();
					istack.pop();
				}
				else if (istack.back().getInstruction() == "functiondef") {
					bytecode << Byte("push", "0");
					bytecode << Byte("return");
					bytecode << Byte("endfunction", istack.back().getData());
					istack.pop();
				}
				else if (istack.back().getInstruction() == "classdef") {
					bytecode << Byte("endclass", istack.back().getData());
					istack.pop();
				}
				i += 1;
			}
			else if (type == TT::RETURN) {
				istack << Instruction("return", "0");
				i += 1;
			}
			else if (name == "returnref") {
				istack << Instruction("returnref", "0");
				i += 1;
			}
			else if (type == TT::ELLIPSIS) {
				if (istack.back().getInstruction() == "functionparams") {
					if (tokens.at(i + 1).getType() == TT::COMMA || tokens.at(i + 1).getType() == TT::RBRACKET) {
						bytecode << Byte("varparam", name);
						if (tokens.at(i + 1).getType() == TT::COMMA)
							i += 2;
						else
							i += 1;
						continue;
					}
				}
				i += 1;
			}
		}
		this->optimise();
	}
}