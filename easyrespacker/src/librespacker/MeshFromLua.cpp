#include "MeshFromLua.h"
#include "PackMesh.h"
#include "UnpackNodeFactory.h"
#include "LuaDataHelper.h"

namespace erespacker
{

void MeshFromLua::Unpack(lua_State* L, PackMesh* mesh)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();
	int base_sym_id = LuaDataHelper::GetIntField(L, "base_sym_id");
	mesh->base_sym = factory->Query(base_sym_id);
	if (!mesh->base_sym) {
		factory->AddUnassigned(base_sym_id, &mesh->base_sym);
	}

	UnpackVertices(mesh->triangles, L);
}

}