#ifndef _LIBRESPACKER_PARTICLE2D_TO_LUA_STRING_H_
#define _LIBRESPACKER_PARTICLE2D_TO_LUA_STRING_H_

#include "NodeToLua.h"
#include "PackParticle2D.h"

namespace librespacker
{

class Particle2DToLuaString : private NodeToLua
{
public:
	static void Pack(const PackParticle2D* p2d, ebuilder::CodeGenerator& gen);

private:
	static void PackPS(const PackParticle2D* p2d, ebuilder::CodeGenerator& gen);
	static void PackComponent(const PackParticle2D::Component& comp, 
		ebuilder::CodeGenerator& gen);

}; // Particle2DToLuaString

}

#endif // _LIBRESPACKER_PARTICLE2D_TO_LUA_STRING_H_