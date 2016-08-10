#include "PackTrail.h"

#include "TrailToLuaString.h"
#include "TrailFromLua.h"

#include "TrailToBin.h"
#include "TrailFromBin.h"

namespace erespacker
{

PackTrail::PackTrail(int id)
	: IPackNode(id)
{
}

void PackTrail::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	TrailToLuaString::Pack(this, gen);
}

void PackTrail::UnpackFromLua(lua_State* L, const std::vector<ee::Image*>& images)
{
	TrailFromLua::Unpack(L, this);
}

int PackTrail::SizeOfPackToBin() const
{
	return TrailToBin::Size(this);
}

void PackTrail::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	TrailToBin::Pack(this, ptr);
}

int PackTrail::SizeOfUnpackFromBin() const
{
	return TrailFromBin::Size(this);
}

void PackTrail::UnpackFromBin(uint8_t** ptr, const std::vector<ee::Image*>& images)
{
	TrailFromBin::Unpack(ptr, this);
}

}