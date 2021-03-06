#include "PackParticle2D.h"

#include "Particle2DToLuaString.h"
#include "Particle2DFromLua.h"

#include "Particle2DToBin.h"
#include "Particle2DFromBin.h"

namespace erespacker
{

PackParticle2D::PackParticle2D(int id)
	: IPackNode(id)
{
}

void PackParticle2D::PackToLuaString(ebuilder::CodeGenerator& gen, 
									 const ee::TexturePacker& tp,
									 float scale) const
{
	return Particle2DToLuaString::Pack(this, gen);
}

void PackParticle2D::UnpackFromLua(lua_State* L, const std::vector<ee::ImagePtr>& images)
{
	Particle2DFromLua::Unpack(L, this);
}

int PackParticle2D::SizeOfPackToBin() const
{
	return Particle2DToBin::Size(this);
}

void PackParticle2D::PackToBin(uint8_t** ptr, 
							   const ee::TexturePacker& tp,
							   float scale) const
{
	Particle2DToBin::Pack(this, ptr);
}

int PackParticle2D::SizeOfUnpackFromBin() const
{
	return Particle2DFromBin::Size(this);
}

void PackParticle2D::UnpackFromBin(uint8_t** ptr, const std::vector<ee::ImagePtr>& images)
{
	Particle2DFromBin::Unpack(ptr, this);
}

}