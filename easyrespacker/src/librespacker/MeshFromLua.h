#ifndef _EASYRESPACKER_MESH_FROM_LUA_H_
#define _EASYRESPACKER_MESH_FROM_LUA_H_

#include "NodeFromLua.h"

namespace erespacker
{

class PackMesh;

class MeshFromLua : public NodeFromLua
{
public:
	static void Unpack(lua_State* L, PackMesh* mesh);

}; // MeshFromLua

}

#endif // _EASYRESPACKER_MESH_FROM_LUA_H_