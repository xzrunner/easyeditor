#include "MeshToLuaString.h"
#include "PackMesh.h"

#include <easybuilder.h>

#include <limits>

namespace lua = ebuilder::lua;

namespace erespacker
{

void MeshToLuaString::Pack(const PackMesh* mesh, ebuilder::CodeGenerator& gen)
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
		lua::assign("base_id", mesh->base->GetSprID()));

	PackVertices(mesh->tri_texcoords, gen, "texcoords", false, 0xffff);
	PackVertices(mesh->tri_vertices, gen, "vertices");

	gen.detab();
	gen.line("},");
}

}