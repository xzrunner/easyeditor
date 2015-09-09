#include "PackParticle3D.h"

#include "Particle3DToLuaString.h"
#include "Particle3DFromLua.h"

#include "Particle3DToBin.h"
#include "Particle3DFromBin.h"

namespace libcoco
{

PackParticle3D::PackParticle3D(int id)
	: IPackNode(id)
{
}

void PackParticle3D::PackToLuaString(ebuilder::CodeGenerator& gen, const d2d::TexturePacker& tp) const
{
	return Particle3DToLuaString::Pack(this, gen);
}

void PackParticle3D::UnpackFromLua(lua_State* L, const std::vector<d2d::Image*>& images)
{
	Particle3DFromLua::Unpack(L, this);
}

int PackParticle3D::SizeOfPackToBin() const
{
	return 0;
}

void PackParticle3D::PackToBin(uint8_t** ptr, const d2d::TexturePacker& tp) const
{

}

int PackParticle3D::SizeOfUnpackFromBin() const
{
	return 0;
}

void PackParticle3D::UnpackFromBin(uint8_t** ptr, const std::vector<d2d::Image*>& images)
{

}

}