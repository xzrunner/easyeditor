#ifndef _EASYRESPACKER_MESH_SPR_TO_BIN_H_
#define _EASYRESPACKER_MESH_SPR_TO_BIN_H_

#include "NodeToBin.h"

namespace erespacker
{

class PackMeshSpr;

class MeshSprToBin : private NodeToBin
{
public:
	static int Size(const PackMeshSpr* mesh);

	static void Pack(const PackMeshSpr* mesh, uint8_t** ptr);

}; // MeshSprToBin

}

#endif // _EASYRESPACKER_MESH_TO_BIN_H_