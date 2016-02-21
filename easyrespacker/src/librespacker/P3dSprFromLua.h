#ifndef _EASYRESPACKER_P3D_SPR_FROM_LUA_H_
#define _EASYRESPACKER_P3D_SPR_FROM_LUA_H_

#include "NodeFromLua.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

namespace erespacker
{

class PackP3dSpr;

class P3dSprFromLua : private NodeFromLua
{
public:
	static void Unpack(lua_State* L, PackP3dSpr* spr);

}; // P3dSprFromLua

}

#endif // _EASYRESPACKER_P3D_SPR_FROM_LUA_H_