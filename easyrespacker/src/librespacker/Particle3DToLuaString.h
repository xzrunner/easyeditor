#ifndef _EASYRESPACKER_PARTICLE3D_TO_LUA_STRING_H_
#define _EASYRESPACKER_PARTICLE3D_TO_LUA_STRING_H_

#include "NodeToLua.h"
#include "PackParticle3D.h"

namespace erespacker
{

class Particle3DToLuaString : private NodeToLua
{
public:
	static void Pack(const PackParticle3D* p3d, ebuilder::CodeGenerator& gen);

private:
	static void PackPS(const PackParticle3D* p3d, ebuilder::CodeGenerator& gen);
	static void PackComponent(const PackParticle3D::Component& comp, 
		ebuilder::CodeGenerator& gen);

}; // Particle3DToLuaString

}

#endif // _EASYRESPACKER_PARTICLE3D_TO_LUA_STRING_H_