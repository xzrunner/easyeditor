#ifndef _EASYRESPACKER_MESH_TO_LUA_STRING_H_
#define _EASYRESPACKER_MESH_TO_LUA_STRING_H_

#include "NodeToLua.h"

namespace erespacker
{

class PackMesh;

class MeshToLuaString : private NodeToLua
{
public:
	static void Pack(const PackMesh* mesh, ebuilder::CodeGenerator& gen);

}; // MeshToLuaString

}

#endif // _EASYRESPACKER_MESH_TO_LUA_STRING_H_