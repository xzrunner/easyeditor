#ifndef _LIBRESPACKER_PARTICLE2D_FROM_LUA_H_
#define _LIBRESPACKER_PARTICLE2D_FROM_LUA_H_

#include "NodeFromLua.h"
#include "PackParticle2D.h"

namespace librespacker
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

#endif // _LIBRESPACKER_PARTICLE2D_FROM_LUA_H_