#ifndef REFERENCE_H
#define REFERENCE_H

namespace sda
{
	class Reference
	{
	private:
		size_t address;
	public:
		size_t& get();
	};
}

#endif