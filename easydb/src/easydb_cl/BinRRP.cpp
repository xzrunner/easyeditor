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
	return Command() + " [json file] [images id file]";
}

void BinRRP::Run(int argc, char *argv[])
{
	// rrpbin E:\debug\character\pack.json

	if (!check_number(this, argc, 4)) return;
	if (!check_file(argv[2])) return;
	if (!check_file(argv[3])) return;

	Trigger(argv[2], argv[3]);
}

void BinRRP::Trigger(const std::string& json_file, 
					 const std::string& image_id_file)
{
	libpacker::GenRegularRectBinary pack(json_file, image_id_file);
	pack.PackToBinary();
}

}