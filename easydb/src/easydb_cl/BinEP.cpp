#include "BinEP.h"
#include "check_params.h"

#include <ee/Exception.h>

#include <epbin.h>

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
	return Command() + " [filename] [type] [output]";
}

void BinEP::Run(int argc, char *argv[])
{
	// epbin E:\debug\sg_ui\ui2 png E:\debug\sg_ui\ui2

	if (!check_number(this, argc, 5)) return;

	Trigger(argv[2], argv[3], argv[4]);
}

void BinEP::Trigger(const std::string& filename, const std::string& type, 
					const std::string& output)
{
	epbin::BinaryEPD epd(filename + ".lua");
	epd.Pack(output + ".epd", true);

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
		throw ee::Exception("BinEP::Trigger unknown type: %s\n", type);
	}
	epbin::BinaryEPP epp(filename, t);
	epp.Pack(output + ".epp");
}

}