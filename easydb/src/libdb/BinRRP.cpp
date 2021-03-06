#include "BinRRP.h"
#include "check_params.h"

#include <easytexpacker.h>

namespace edb
{

std::string BinRRP::Command() const
{
	return "bin-rrp";
}

std::string BinRRP::Description() const
{
	return "pack rrp json file to binary";
}

std::string BinRRP::Usage() const
{
	return Command() + " [json file] [images id file]";
}

int BinRRP::Run(int argc, char *argv[])
{
	// bin-rrp E:\debug\character\pack.json E:\debug\character\id_images.txt

	if (!check_number(this, argc, 4)) return -1;
	if (!check_file(argv[2])) return -1;
	if (!check_file(argv[3])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void BinRRP::Trigger(const std::string& json_file, 
					 const std::string& image_id_file)
{
	etexpacker::GenRegularRectBinary pack(json_file, image_id_file);
	pack.PackToBinary();
}

}