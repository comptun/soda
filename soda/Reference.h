#ifndef REFERENCE_H
#define REFERENCE_H

namespace sda
{
	class Reference
	{
	private:
		size_t m_Address;
		size_t m_StackFrame;
	public:
		size_t& address();
		size_t& stackFrame();
		Reference(size_t m_Address, int m_StackFrame);
		Reference();
	};
}

#endif