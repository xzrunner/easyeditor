#include "BinRRP.h"
#include "check_params.h"

#include <easypacker.h>

namespace edb
{

std::string BinRRP::Command() const
{
	return "rrpbin";
}

std::string BinRRP::Description() const
{
	return "pack rrp json file to binary";
}

std::string BinRRP::Usage() const
{
	return Command() + " [src json file]";
}

void BinRRP::Run(int argc, char *argv[])
{
	// rrpbin E:\debug\character\pack.json

	if (!check_number(this, argc, 3)) return;
	if (!check_file(argv[2])) return;

	Trigger(argv[2]);
}

void BinRRP::Trigger(const std::string& filepath)
{
	libpacker::GenRegularRectBinary pack(filepath);
	pack.PackToBinary();
}

}