#include "ResUnpacker.h"

#include "UnpackFromLua.h"
#include "UnpackFromBin.h"

namespace librespacker
{

void ResUnpacker::UnpackLua(const std::string& filepath, const std::vector<d2d::Image*>& images)
{
	UnpackFromLua::Unpack(filepath, images);
}

void ResUnpacker::UnpackBin(const std::string& filepath, const std::vector<d2d::Image*>& images)
{
	UnpackFromBin::Unpack(filepath, images);
}

}