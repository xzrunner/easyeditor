#include "BinEP.h"
#include "check_params.h"

#include <epbin.h>
#include <drag2d.h>

namespace edb
{

std::string BinEP::Command() const
{
	return "epbin";
}

std::string BinEP::Description() const
{
	return "pack ejoy2d's .lua and images to binary";
}

std::string BinEP::Usage() const
{
	return Command() + " [dir] [filename] [type] [output]";
}

void BinEP::Run(int argc, char *argv[])
{
	// epbin E:\debug\sg_ui ui2 png E:\debug\sg_ui\ui2.epp

	if (!check_number(this, argc, 6)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2], argv[3], argv[4], argv[5]);
}

void BinEP::Trigger(const std::string& dir, const std::string& filename,
					const std::string& type, const std::string& output)
{
	epbin::TextureType t;
	if (type == "png4") {
		t = epbin::TT_PNG4;
	} else if (type == "png8") {
		t = epbin::TT_PNG8;
	} else if (type == "pvr") {
		t = epbin::TT_PVR;
	} else if (type == "pkm") {
		t = epbin::TT_PKM;
	} else {
		throw d2d::Exception("BinEP::Trigger unknown type: %s\n", type);
	}

	epbin::BinaryEPP epp(dir, filename, t);
	epp.Pack(output);
}

}