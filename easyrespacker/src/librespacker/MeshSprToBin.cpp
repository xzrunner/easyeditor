#include "MeshSprToBin.h"
#include "PackMeshSpr.h"
#include "NodeToBin.h"
#include "pack_unpack.h"

// for TYPE_MESH
#include <spritepack.h>

namespace erespacker
{

int MeshSprToBin::Size(const PackMeshSpr* mesh)
{
	int sz = 0;
	sz += sizeof(uint16_t);				// id
	sz += sizeof(uint8_t);				// type
	sz += sizeof(uint16_t);				// mesh id
	sz += sizeof(uint16_t);				// base id
	SizeVertices(mesh->trans_pairs);	// triangles
	return sz;
}

void MeshSprToBin::Pack(const PackMeshSpr* mesh, uint8_t** ptr)
{
	uint16_t id = mesh->GetSprID();
	pack(id, ptr);

	uint8_t type = TYPE_MESH_SPR;
	pack(type, ptr);

	assert(mesh->mesh);
	uint16_t mesh_id = mesh->mesh->GetSprID();
	pack(mesh_id, ptr);	
 
	assert(mesh->base);
	uint16_t base_id = mesh->base->GetSprID();
	pack(base_id, ptr);

	PackVertices(mesh->trans_pairs, ptr);
}

}