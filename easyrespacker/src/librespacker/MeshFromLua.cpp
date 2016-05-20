#include "MeshFromLua.h"
#include "PackMesh.h"
#include "UnpackNodeFactory.h"
#include "LuaDataHelper.h"

namespace erespacker
{

void MeshFromLua::Unpack(lua_State* L, PackMesh* mesh)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();
	int base_id = LuaDataHelper::GetIntField(L, "base_id");
	mesh->base = factory->Query(base_id);
	if (!mesh->base) {
		factory->AddUnassigned(base_id, &mesh->base);
	}

	UnpackVertices(mesh->triangles, L);
}

}