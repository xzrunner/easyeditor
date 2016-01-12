#ifndef _LIBRESPACKER_SHAPE_FROM_LUA_H_
#define _LIBRESPACKER_SHAPE_FROM_LUA_H_

#include "PackShape.h"

namespace librespacker
{

class ShapeFromLua
{
public:
	static void Unpack(lua_State* L, PackShape* shape);

}; // ShapeFromLua

}

#endif // _LIBRESPACKER_SHAPE_FROM_LUA_H_