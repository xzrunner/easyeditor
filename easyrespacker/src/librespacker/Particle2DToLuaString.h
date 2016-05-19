#ifndef _EASYRESPACKER_PARTICLE2D_TO_LUA_STRING_H_
#define _EASYRESPACKER_PARTICLE2D_TO_LUA_STRING_H_

#include "NodeToLuaString.h"
#include "PackParticle2D.h"

namespace erespacker
{

class Particle2DToLuaString : private NodeToLuaString
{
public:
	static void Pack(const PackParticle2D* p2d, ebuilder::CodeGenerator& gen);

private:
	static void PackPS(const PackParticle2D* p2d, ebuilder::CodeGenerator& gen);
	static void PackComponent(const PackParticle2D::Component& comp, 
		ebuilder::CodeGenerator& gen);

}; // Particle2DToLuaString

}

#endif // _EASYRESPACKER_PARTICLE2D_TO_LUA_STRING_H_