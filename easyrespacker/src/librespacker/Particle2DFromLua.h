#ifndef _EASYRESPACKER_PARTICLE2D_FROM_LUA_H_
#define _EASYRESPACKER_PARTICLE2D_FROM_LUA_H_

#include "NodeFromLua.h"
#include "PackParticle2D.h"

namespace erespacker
{

class Particle2DFromLua : public NodeFromLua
{
public:
	static void Unpack(lua_State* L, PackParticle2D* p2d);

private:
	static void UnpackComponents(lua_State* L, PackParticle2D* p2d);
	static void UnpackBody(lua_State* L, PackParticle2D* p2d);

}; // Particle2DFromLua

}

#endif // _EASYRESPACKER_PARTICLE2D_FROM_LUA_H_