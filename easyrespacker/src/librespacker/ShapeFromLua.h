#ifndef _EASYRESPACKER_SHAPE_FROM_LUA_H_
#define _EASYRESPACKER_SHAPE_FROM_LUA_H_

#include "PackShape.h"

namespace erespacker
{

class ShapeFromLua
{
public:
	static void Unpack(lua_State* L, PackShape* shape);

}; // ShapeFromLua

}

#endif // _EASYRESPACKER_SHAPE_FROM_LUA_H_