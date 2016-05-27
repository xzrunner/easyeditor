#include "PackMask.h"

#include "MaskToLuaString.h"
#include "MaskFromLua.h"

#include "MaskToBin.h"
#include "MaskFromBin.h"

namespace erespacker
{

PackMask::PackMask(int id)
	: IPackNode(id)
{
}

void PackMask::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	MaskToLuaString::Pack(this, gen);
}

void PackMask::UnpackFromLua(lua_State* L, const std::vector<ee::Image*>& images)
{
	MaskFromLua::Unpack(L, this);
}

int PackMask::SizeOfPackToBin() const
{
	return MaskToBin::Size(this);
}

void PackMask::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	MaskToBin::Pack(this, ptr);
}

int PackMask::SizeOfUnpackFromBin() const
{
	return MaskFromBin::Size();
}

void PackMask::UnpackFromBin(uint8_t** ptr, const std::vector<ee::Image*>& images)
{
	MaskFromBin::Unpack(ptr, this);
}

}