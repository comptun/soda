#ifndef BYTE_H
#define BYTE_H

#include <vector>
#include <string>

namespace sda
{
	class Byte
	{
	private:
		std::string opcode;
		std::string data;
	public:
		void setOpcode(std::string const& opcode);
		void setData(std::string const& data);
		std::string& getOpcode();
		std::string& getData();
		Byte(std::string const& opcode, std::string const& data);
		Byte();
	};

	class Bytecode
	{
	private:
		std::vector<Byte> bytecode;
	public:
		void push_byte(Byte instruction);
		friend Bytecode& operator<<(Bytecode& out, Byte byte);
		friend std::ostream& operator<<(std::ostream& out, Bytecode bytes);
		Byte& get(size_t index);
		Byte& get();
	};
}

#endif
