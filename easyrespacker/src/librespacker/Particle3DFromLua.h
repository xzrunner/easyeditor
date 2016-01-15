#ifndef _LIBRESPACKER_PARTICLE3D_FROM_LUA_H_
#define _LIBRESPACKER_PARTICLE3D_FROM_LUA_H_

#include "NodeFromLua.h"
#include "PackParticle3D.h"

namespace librespacker
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

#endif // _LIBRESPACKER_PARTICLE3D_FROM_LUA_H_