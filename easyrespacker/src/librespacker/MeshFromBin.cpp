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

	uint16_t base_id;
	unpack(base_id, ptr);
	mesh->base = factory->Query(base_id);
	if (!mesh->base) {
		factory->AddUnassigned(base_id, &mesh->base);
	}

	UnpackVertices(mesh->triangles, ptr);
}

}