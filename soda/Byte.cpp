#include "Byte.h"
#include <iostream>

namespace sda
{
	void Byte::setOpcode(std::string const& opcode)
	{
		this->opcode = opcode;
	}
	void Byte::setData(std::string const& data)
	{
		this->data = data;
	}
	std::string& Byte::getOpcode()
	{
		return this->opcode;
	}
	std::string& Byte::getData()
	{
		return this->data;
	}
	Byte::Byte(std::string const& opcode, std::string const& data)
	{
		this->opcode = opcode;
		this->data = data;
	}
	Byte::Byte()
	{
		this->opcode = "EMPTY";
		this->data = "EMPTY";
	}

	void Bytecode::push_byte(Byte instruction)
	{
		this->bytecode.push_back(instruction);
	}
	Bytecode& operator<<(Bytecode& out, Byte byte)
	{
		out.push_byte(byte);
		return out;
	}
	std::ostream& operator<<(std::ostream& out, Bytecode bytes)
	{
		/*for (Byte& byte : bytes.bytecode) {
			std::cout << byte.getOpcode() << " " << byte.getData() << "\n";
		}*/
		for (size_t i = 0; i < bytes.size(); ++i) {
			std::cout << i << " " << bytes.at(i).getOpcode() << " " << bytes.at(i).getData() << "\n";
		}
		return out;
	}
	size_t Bytecode::size()
	{
		return this->bytecode.size();
	}
	Byte& Bytecode::at(size_t index)
	{
		return this->bytecode.at(index);
	}
	Byte& Bytecode::back()
	{
		return this->bytecode.back();
	}
}