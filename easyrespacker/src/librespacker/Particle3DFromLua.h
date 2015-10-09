#ifndef _LIBRESPACKER_PARTICLE3D_FROM_LUA_H_
#define _LIBRESPACKER_PARTICLE3D_FROM_LUA_H_

#include "PackParticle3D.h"

namespace librespacker
{

class Particle3DFromLua
{
public:
	static void Unpack(lua_State* L, PackParticle3D* p3d);

private:
	static void UnpackComponents(lua_State* L, PackParticle3D* p3d);
	static void UnpackBody(lua_State* L, PackParticle3D* p3d);

	static float TransTime(int time);
	static float TransDegree(int deg);
	static float TransFloatX100(int f);

}; // Particle3DFromLua

}

#endif // _LIBRESPACKER_PARTICLE3D_FROM_LUA_H_