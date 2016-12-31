#include "MeshSprFromBin.h"
#include "PackMeshSpr.h"
#include "pack_unpack.h"
#include "UnpackNodeFactory.h"

// for SIZEOF_MESH
#include "spritepack.h"

namespace erespacker
{

int MeshSprFromBin::Size(const PackMeshSpr* mesh)
{
	return SIZEOF_MESH_SPR + SizeVertices(mesh->trans_pairs);
}

void MeshSprFromBin::Unpack(uint8_t** ptr, PackMeshSpr* mesh)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	uint16_t mesh_id;
	unpack(mesh_id, ptr);
	mesh->mesh = factory->Query(mesh_id);
	if (!mesh->base) {
		factory->AddUnassigned(mesh_id, &mesh->mesh);
	}

	uint16_t base_id;
	unpack(base_id, ptr);
	mesh->base = factory->Query(base_id);
	if (!mesh->base) {
		factory->AddUnassigned(base_id, &mesh->base);
	}

	UnpackVertices(mesh->trans_pairs, ptr);
}

}