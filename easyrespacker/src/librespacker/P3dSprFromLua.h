#ifndef _LIBRESPACKER_P3D_SPR_FROM_LUA_H_
#define _LIBRESPACKER_P3D_SPR_FROM_LUA_H_

#include "NodeFromLua.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

namespace librespacker
{

class PackP3dSpr;

class P3dSprFromLua : private NodeFromLua
{
public:
	static void Unpack(lua_State* L, PackP3dSpr* spr);

}; // P3dSprFromLua

}

#endif // _LIBRESPACKER_P3D_SPR_FROM_LUA_H_