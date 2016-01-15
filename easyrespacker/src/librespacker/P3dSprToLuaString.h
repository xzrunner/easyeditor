#ifndef _LIBRESPACKER_P3D_SPR_TO_LUA_STRING_H_
#define _LIBRESPACKER_P3D_SPR_TO_LUA_STRING_H_

#include "NodeToLua.h"

namespace ebuilder { class CodeGenerator; }

namespace librespacker
{

class PackP3dSpr;

class P3dSprToLuaString : private NodeToLua
{
public:
	static void Pack(const PackP3dSpr* p3d, ebuilder::CodeGenerator& gen);

}; // P3dSprToLuaString

}

#endif // _LIBRESPACKER_P3D_SPR_TO_LUA_STRING_H_