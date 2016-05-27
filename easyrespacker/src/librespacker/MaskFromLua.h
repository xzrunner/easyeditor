#ifndef _EASYRESPACKER_MASK_FROM_LUA_H_
#define _EASYRESPACKER_MASK_FROM_LUA_H_

#include "NodeFromLua.h"

namespace erespacker
{

class PackMask;

class MaskFromLua : public NodeFromLua
{
public:
	static void Unpack(lua_State* L, PackMask* mask);

}; // MaskFromLua

}

#endif // _EASYRESPACKER_MASK_FROM_LUA_H_