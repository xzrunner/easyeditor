#include "PackImageEPT.h"
#include "check_params.h"

namespace edb
{

std::string PackImageEPT::Command() const
{
	return "pack-img-ept";
}

std::string PackImageEPT::Description() const
{
	return "pack image ept file";
}

// pack-img-ept images\distortion_map.png output\distortion_map.ept png
std::string PackImageEPT::Usage() const
{
	return Command() + " [src] [dst] [output type]";
}

int PackImageEPT::Run(int argc, char* argv[])
{
	if (!check_number(this, argc, 3)) return -1;

	erespacker::TextureType type = erespacker::TT_PNG8;
	char* stype = argv[4];
	if (strcmp(stype, "png") == 0) {
		type = erespacker::TT_PNG8;
	} else if (strcmp(stype, "pvr") == 0) {
		type = erespacker::TT_PVR;
	}
	Trigger(argv[2], argv[3], type);

	return 0;
}

void PackImageEPT::Trigger(const std::string& src, const std::string& dst, 
						   erespacker::TextureType type)
{
	erespacker::PackToBin::PackEPT(src, dst, type);
}

}