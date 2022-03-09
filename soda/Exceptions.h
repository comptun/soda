#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "ExceptionType.h"

namespace sda
{
	class Exceptions
	{
	private:
		bool exceptionCaught;
		ET caughtException;
	public:
		void exception(ET exceptionType);
		bool hasCaughtException();
		ET getCaughtException();
		Exceptions();
	};
}

#endif