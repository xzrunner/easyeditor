#ifndef _EASYRESPACKER_MESH_SPR_TO_LUA_STRING_H_
#define _EASYRESPACKER_MESH_SPR_TO_LUA_STRING_H_

#include "NodeToLuaString.h"

namespace erespacker
{

class PackMeshSpr;

class MeshSprToLuaString : private NodeToLuaString
{
public:
	static void Pack(const PackMeshSpr* mesh, ebuilder::CodeGenerator& gen);

}; // MeshSprToLuaString

}

#endif // _EASYRESPACKER_MESH_SPR_TO_LUA_STRING_H_