#include "PackShape.h"

#include "ShapeToLuaString.h"
#include "ShapeFromLua.h"

#include "ShapeToBin.h"
#include "ShapeFromBin.h"

namespace librespacker
{

PackShape::PackShape(int id)
	: IPackNode(id)
{
}

void PackShape::PackToLuaString(ebuilder::CodeGenerator& gen, 
								const d2d::TexturePacker& tp,
								float scale) const
{
	ShapeToLuaString::Pack(this, gen);
}

void PackShape::UnpackFromLua(lua_State* L, const std::vector<d2d::Image*>& images)
{
	ShapeFromLua::Unpack(L, this);
}

int PackShape::SizeOfPackToBin() const
{
	return ShapeToBin::Size(this);
}

void PackShape::PackToBin(uint8_t** ptr, 
						  const d2d::TexturePacker& tp,
						  float scale) const
{
	ShapeToBin::Pack(this, ptr);
}

int PackShape::SizeOfUnpackFromBin() const
{
	return ShapeFromBin::Size(this);
}

void PackShape::UnpackFromBin(uint8_t** ptr, const std::vector<d2d::Image*>& images)
{
	ShapeFromBin::Unpack(ptr, this);
}

}