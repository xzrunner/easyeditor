#include "MeshSprToLuaString.h"
#include "PackMeshSpr.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace erespacker
{

void MeshSprToLuaString::Pack(const PackMeshSpr* mesh, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + mesh->GetFilepath());

	lua::assign_with_end(gen, "type", "\"mesh\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(mesh->GetSprID()));
	if (!mesh->export_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + mesh->export_name + "\"");
	}

	lua::connect(gen, 1, 
		lua::assign("mesh_id", mesh->mesh->GetSprID()));
	lua::connect(gen, 1, 
		lua::assign("base_id", mesh->base->GetSprID()));
	
	PackVertices(mesh->trans_pairs, gen);	

	gen.detab();
	gen.line("},");
}

}