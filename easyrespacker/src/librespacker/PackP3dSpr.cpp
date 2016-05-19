#include "PackP3dSpr.h"

#include "P3dSprToLuaString.h"
#include "P3dSprFromLua.h"

#include "P3dSprToBin.h"
#include "P3dSprFromBin.h"

namespace erespacker
{

PackP3dSpr::PackP3dSpr(int id)
	: IPackNode(id)
{
}

void PackP3dSpr::PackToLuaString(ebuilder::CodeGenerator& gen, 
								 const ee::TexturePacker& tp,
								 float scale) const
{
	return P3dSprToLuaString::Pack(this, gen);
}

void PackP3dSpr::UnpackFromLua(lua_State* L, const std::vector<ee::Image*>& images)
{
	P3dSprFromLua::Unpack(L, this);
}

int PackP3dSpr::SizeOfPackToBin() const
{
	return P3dSprToBin::Size();
}

void PackP3dSpr::PackToBin(uint8_t** ptr, 
						   const ee::TexturePacker& tp,
						   float scale) const
{
	P3dSprToBin::Pack(this, ptr);
}

int PackP3dSpr::SizeOfUnpackFromBin() const
{
	return P3dSprFromBin::Size();
}

void PackP3dSpr::UnpackFromBin(uint8_t** ptr, const std::vector<ee::Image*>& images)
{
	P3dSprFromBin::Unpack(ptr, this);
}

}