#include "Reference.h"

namespace sda
{
	size_t& Reference::get()
	{
		return this->address;
	}
}