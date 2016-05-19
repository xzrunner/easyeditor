#include "MeshFromBin.h"
#include "PackMesh.h"
#include "pack_unpack.h"
#include "UnpackNodeFactory.h"

// for SIZEOF_MESH
#include <spritepack.h>

namespace erespacker
{

int MeshFromBin::Size(const PackMesh* mesh)
{
	return SIZEOF_MESH + SizeVertices(mesh->triangles);
}

void MeshFromBin::Unpack(uint8_t** ptr, PackMesh* mesh)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	uint16_t base_sym_id;
	unpack(base_sym_id, ptr);
	mesh->base_sym = factory->Query(base_sym_id);
	if (!mesh->base_sym) {
		factory->AddUnassigned(base_sym_id, &mesh->base_sym);
	}

	UnpackVertices(mesh->triangles, ptr);
}

}