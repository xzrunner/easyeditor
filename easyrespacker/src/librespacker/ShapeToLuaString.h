#ifndef _EASYRESPACKER_SHAPE_TO_LUA_STRING_H_
#define _EASYRESPACKER_SHAPE_TO_LUA_STRING_H_

#include "PackShape.h"

namespace erespacker
{

class ShapeToLuaString
{
public:
	static void Pack(const PackShape* shape, ebuilder::CodeGenerator& gen);

}; // ShapeToLuaString

}

#endif // _EASYRESPACKER_SHAPE_TO_LUA_STRING_H_