#ifndef _LIBRESPACKER_SHAPE_TO_LUA_STRING_H_
#define _LIBRESPACKER_SHAPE_TO_LUA_STRING_H_

#include "PackShape.h"

namespace librespacker
{

class ShapeToLuaString
{
public:
	static void Pack(const PackShape* shape, ebuilder::CodeGenerator& gen);

}; // ShapeToLuaString

}

#endif // _LIBRESPACKER_SHAPE_TO_LUA_STRING_H_