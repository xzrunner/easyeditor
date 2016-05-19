#ifndef _EASYRESPACKER_MESH_FROM_BIN_H_
#define _EASYRESPACKER_MESH_FROM_BIN_H_

#include "NodeFromBin.h"

namespace erespacker
{

class PackMesh;

class MeshFromBin : private NodeFromBin
{
public:
	static int Size(const PackMesh* mesh);

	static void Unpack(uint8_t** ptr, PackMesh* mesh);

}; // MeshFromBin

}

#endif // _EASYRESPACKER_MESH_FROM_BIN_H_