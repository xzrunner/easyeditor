#include "PackAnchor.h"

namespace esprpacker
{

PackAnchor::PackAnchor()
{
	SetFilepath("anchor");
}

void PackAnchor::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	int zz = 0;
}

int PackAnchor::SizeOfUnpackFromBin() const
{
	return 0;
}

int PackAnchor::SizeOfPackToBin() const
{
	return 0;
}

void PackAnchor::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	int zz = 0;
}

}