#ifndef _EASYRESPACKER_P3D_SPR_TO_LUA_STRING_H_
#define _EASYRESPACKER_P3D_SPR_TO_LUA_STRING_H_

#include "NodeToLuaString.h"

namespace ebuilder { class CodeGenerator; }

namespace erespacker
{

class PackP3dSpr;

class P3dSprToLuaString : private NodeToLuaString
{
public:
	static void Pack(const PackP3dSpr* p3d, ebuilder::CodeGenerator& gen);

}; // P3dSprToLuaString

}

#endif // _EASYRESPACKER_P3D_SPR_TO_LUA_STRING_H_