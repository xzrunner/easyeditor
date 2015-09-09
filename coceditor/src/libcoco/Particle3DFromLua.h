#ifndef _LIBCOCO_PARTICLE3D_FROM_LUA_H_
#define _LIBCOCO_PARTICLE3D_FROM_LUA_H_

#include "PackParticle3D.h"

namespace libcoco
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

#endif // _LIBCOCO_PARTICLE3D_FROM_LUA_H_