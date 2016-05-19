#ifndef _EASYRESPACKER_MESH_SPR_FROM_BIN_H_
#define _EASYRESPACKER_MESH_SPR_FROM_BIN_H_

#include "NodeFromBin.h"

namespace erespacker
{

class PackMeshSpr;

class MeshSprFromBin : private NodeFromBin
{
public:
	static int Size(const PackMeshSpr* mesh);

	static void Unpack(uint8_t** ptr, PackMeshSpr* mesh);

}; // MeshSprFromBin

}

#endif // _EASYRESPACKER_MESH_SPR_FROM_BIN_H_