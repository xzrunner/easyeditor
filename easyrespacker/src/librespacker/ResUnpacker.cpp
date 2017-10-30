#include "ResUnpacker.h"

#include "UnpackFromLua.h"
#include "UnpackFromBin.h"

namespace erespacker
{

void ResUnpacker::UnpackLua(const std::string& filepath, const std::vector<ee::ImagePtr>& images)
{
	UnpackFromLua::Unpack(filepath, images);
}

void ResUnpacker::UnpackBin(const std::string& filepath, const std::vector<ee::ImagePtr>& images)
{
	UnpackFromBin::Unpack(filepath, images);
}

}