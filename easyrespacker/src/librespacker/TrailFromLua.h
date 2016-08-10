#ifndef _EASYRESPACKER_TRAIL_FROM_LUA_H_
#define _EASYRESPACKER_TRAIL_FROM_LUA_H_

#include "NodeFromLua.h"
#include "PackTrail.h"

namespace erespacker
{

class TrailFromLua : private NodeFromLua
{
public:
	static void Unpack(lua_State* L, PackTrail* trail);

private:
	static void UnpackBody(lua_State* L, PackTrail* trail);
	static void UnpackComponents(lua_State* L, PackTrail* trail);

}; // TrailFromLua

}

#endif // _EASYRESPACKER_TRAIL_FROM_LUA_H_