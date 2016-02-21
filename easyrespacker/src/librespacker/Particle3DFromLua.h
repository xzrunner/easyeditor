#ifndef _EASYRESPACKER_PARTICLE3D_FROM_LUA_H_
#define _EASYRESPACKER_PARTICLE3D_FROM_LUA_H_

#include "NodeFromLua.h"
#include "PackParticle3D.h"

namespace erespacker
{

class Particle3DFromLua : private NodeFromLua
{
public:
	static void Unpack(lua_State* L, PackParticle3D* p3d);

private:
	static void UnpackComponents(lua_State* L, PackParticle3D* p3d);
	static void UnpackBody(lua_State* L, PackParticle3D* p3d);

}; // Particle3DFromLua

}

#endif // _EASYRESPACKER_PARTICLE3D_FROM_LUA_H_