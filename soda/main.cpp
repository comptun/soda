#include "Soda.h"

int main(int argc, char* argv[])
{
	sda::Soda soda;
	return soda.main(std::string(argv[1]));
}