#include "ResUnpacker.h"

#include "UnpackFromLua.h"
#include "UnpackFromBin.h"

namespace librespacker
{

void ResUnpacker::UnpackLua(const std::string& filepath, const std::vector<ee::Image*>& images)
{
	UnpackFromLua::Unpack(filepath, images);
}

void ResUnpacker::UnpackBin(const std::string& filepath, const std::vector<ee::Image*>& images)
{
	UnpackFromBin::Unpack(filepath, images);
}

}