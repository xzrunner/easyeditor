#ifndef _EASYRESPACKER_MESH_SPR_FROM_LUA_H_
#define _EASYRESPACKER_MESH_SPR_FROM_LUA_H_

#include "NodeFromLua.h"

namespace erespacker
{

class PackMeshSpr;

class MeshSprFromLua : public NodeFromLua
{
public:
	static void Unpack(lua_State* L, PackMeshSpr* mesh);

}; // MeshSprFromLua

}

#endif // _EASYRESPACKER_MESH_SPR_FROM_LUA_H_