#ifndef _LIBCOCO_PARTICLE2D_FROM_LUA_H_
#define _LIBCOCO_PARTICLE2D_FROM_LUA_H_

#include "PackParticle2D.h"

namespace libcoco
{

class Particle2DFromLua
{
public:
	static void Unpack(lua_State* L, PackParticle2D* p2d);

private:
	static void UnpackComponents(lua_State* L, PackParticle2D* p2d);
	static void UnpackBody(lua_State* L, PackParticle2D* p2d);

	static float TransTime(int time);
	static float TransDegree(int deg);
	static float TransFloatX100(int f);

}; // Particle2DFromLua

}

#endif // _LIBCOCO_PARTICLE2D_FROM_LUA_H_