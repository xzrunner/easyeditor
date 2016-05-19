#ifndef _EASYRESPACKER_MESH_TO_LUA_STRING_H_
#define _EASYRESPACKER_MESH_TO_LUA_STRING_H_

#include "NodeToLuaString.h"

namespace erespacker
{

class PackMesh;

class MeshToLuaString : private NodeToLuaString
{
public:
	static void Pack(const PackMesh* mesh, ebuilder::CodeGenerator& gen);

}; // MeshToLuaString

}

#endif // _EASYRESPACKER_MESH_TO_LUA_STRING_H_