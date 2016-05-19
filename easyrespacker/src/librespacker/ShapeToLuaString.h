#ifndef _EASYRESPACKER_SHAPE_TO_LUA_STRING_H_
#define _EASYRESPACKER_SHAPE_TO_LUA_STRING_H_

#include "NodeToLuaString.h"

namespace erespacker
{

class PackShape;

class ShapeToLuaString : private NodeToLuaString
{
public:
	static void Pack(const PackShape* shape, ebuilder::CodeGenerator& gen);

}; // ShapeToLuaString

}

#endif // _EASYRESPACKER_SHAPE_TO_LUA_STRING_H_