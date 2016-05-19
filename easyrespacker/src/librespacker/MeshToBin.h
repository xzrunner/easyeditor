#ifndef _EASYRESPACKER_MESH_TO_BIN_H_
#define _EASYRESPACKER_MESH_TO_BIN_H_

#include "NodeToBin.h"

namespace erespacker
{

class PackMesh;

class MeshToBin : private NodeToBin
{
public:
	static int Size(const PackMesh* mesh);

	static void Pack(const PackMesh* mesh, uint8_t** ptr);

}; // MeshToBin

}

#endif // _EASYRESPACKER_MESH_TO_BIN_H_