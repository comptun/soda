#include "Exceptions.h"

namespace sda
{
	void Exceptions::exception(ET exceptionType)
	{
		this->caughtException = exceptionType;
		this->exceptionCaught = true;
	}
	bool Exceptions::hasCaughtException()
	{
		return this->exceptionCaught;
	}
	ET Exceptions::getCaughtException()
	{
		return this->caughtException;
	}
	Exceptions::Exceptions()
	{
		this->caughtException = ET();
		this->exceptionCaught = false;
	}
}