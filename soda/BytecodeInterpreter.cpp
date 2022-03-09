#include "BytecodeInterpreter.h"

namespace sda
{
	std::string BytecodeInterpreter::getType(std::string const& str)
	{
		bool isFloat = false;
		for (size_t i = 0; i < str.size(); ++i) {
			if (!std::isdigit(str.at(i)) && str.at(i) != '.')
				return "string";
			if (str.at(i) == '.')
				isFloat = true;
		}
		if (isFloat)
			return "float";
		return "int";
	}

	void BytecodeInterpreter::push(std::string const& data)
	{
		if (this->getType(data) == "int")
			this->stack.push_back(std::stoll(data));
		else if (this->getType(data) == "float")
			this->stack.push_back(std::stold(data));
		else if (this->getType(data) == "string")
			this->stack.push_back(data);
		else
			this->exception(ET::UNKNOWN_TYPE);
	}

	void BytecodeInterpreter::interpret(Bytecode& bytecode)
	{
		for (size_t i = 0; i < bytecode.size(); ++i) {

			std::string opcode = bytecode.at(i).getOpcode();
			std::string data = bytecode.at(i).getData();

			if (opcode == "pop") {
				this->stack.pop_back();
			}
			else if (opcode == "push") {
				this->push(data);
			}
		}
	}
}