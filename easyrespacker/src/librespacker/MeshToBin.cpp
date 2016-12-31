#include "MeshToBin.h"
#include "PackMesh.h"
#include "NodeToBin.h"
#include "pack_unpack.h"

// for TYPE_MESH
#include "spritepack.h"

#include <limits>

namespace erespacker
{

int MeshToBin::Size(const PackMesh* mesh)
{
	int sz = 0;
	sz += sizeof(uint16_t);						// id
	sz += sizeof(uint8_t);						// type
	sz += sizeof(uint16_t);						// base sym id
	sz += SizeVertices(mesh->tri_texcoords);	// texcoords
	sz += SizeVertices(mesh->tri_vertices);		// vertices
	return sz;
}

void MeshToBin::Pack(const PackMesh* mesh, uint8_t** ptr)
{
	uint16_t id = mesh->GetSprID();
	pack(id, ptr);

	uint8_t type = TYPE_MESH;
	pack(type, ptr);
 
	assert(mesh->base);
	uint16_t base_id = mesh->base->GetSprID();
	pack(base_id, ptr);

	PackVertices(mesh->tri_texcoords, ptr, false, 0xffff);
	PackVertices(mesh->tri_vertices, ptr);
}

}