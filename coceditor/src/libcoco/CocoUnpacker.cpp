#include "CocoUnpacker.h"

#include "UnpackFromLua.h"
#include "UnpackFromBin.h"

namespace libcoco
{

void CocoUnpacker::UnpackLua(const std::string& filepath, const std::vector<d2d::Image*>& images)
{
	UnpackFromLua::Unpack(filepath, images);
}

void CocoUnpacker::UnpackBin(const std::string& filepath, const std::vector<d2d::Image*>& images)
{
	UnpackFromBin::Unpack(filepath, images);
}

}