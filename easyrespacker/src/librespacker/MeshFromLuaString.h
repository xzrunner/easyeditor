#ifndef _EASYRESPACKER_MESH_FROM_LUA_STRING_H_
#define _EASYRESPACKER_MESH_FROM_LUA_STRING_H_

#include "NodeFromLua.h"

namespace erespacker
{

class PackMesh;

class MeshFromLuaString : public NodeFromLua
{
public:
	static void Unpack(lua_State* L, PackMesh* mesh);

}; // MeshFromLuaString

}

#endif // _EASYRESPACKER_MESH_FROM_LUA_STRING_H_