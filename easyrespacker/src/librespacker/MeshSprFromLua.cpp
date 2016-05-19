#include "MeshSprFromLua.h"
#include "PackMeshSpr.h"
#include "UnpackNodeFactory.h"
#include "LuaDataHelper.h"

namespace erespacker
{

void MeshSprFromLua::Unpack(lua_State* L, PackMeshSpr* mesh)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	int mesh_id = LuaDataHelper::GetIntField(L, "mesh_id");
	mesh->base = factory->Query(mesh_id);
	if (!mesh->base) {
		factory->AddUnassigned(mesh_id, &mesh->base);
	}

	int base_id = LuaDataHelper::GetIntField(L, "base_id");
	mesh->base = factory->Query(base_id);
	if (!mesh->base) {
		factory->AddUnassigned(base_id, &mesh->base);
	}

	UnpackVertices(mesh->trans_pairs, L);
}

}