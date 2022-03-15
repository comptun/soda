#include "Reference.h"

namespace sda
{
	size_t& Reference::address()
	{
		return this->m_Address;
	}
	size_t& Reference::stackFrame()
	{
		return this->m_StackFrame;
	}
	Reference::Reference(size_t m_Address, int m_StackFrame)
	{
		this->m_Address = m_Address;
		this->m_StackFrame = m_StackFrame;
	}
	Reference::Reference()
	{
		this->m_Address = 0;
		this->m_StackFrame = 0;
	}
}