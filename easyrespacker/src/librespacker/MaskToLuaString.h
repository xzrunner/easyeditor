#ifndef _EASYRESPACKER_MASK_TO_LUA_STRING_H_
#define _EASYRESPACKER_MASK_TO_LUA_STRING_H_

#include "NodeToLuaString.h"

namespace erespacker
{

class PackMask;

class MaskToLuaString : private NodeToLuaString
{
public:
	static void Pack(const PackMask* mask, ebuilder::CodeGenerator& gen);

}; // MaskToLuaString

}

#endif // _EASYRESPACKER_MASK_TO_LUA_STRING_H_