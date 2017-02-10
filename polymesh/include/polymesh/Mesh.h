#ifndef _POLYMESH_MESH_H_
#define _POLYMESH_MESH_H_

#include "MeshType.h"

namespace pm
{

class MeshData;

class Mesh
{
public:
	virtual ~Mesh() {}
	virtual MeshType Type() const = 0;
	virtual const MeshData* GetMeshData() const = 0;
	
}; // Mesh

}

#endif // _POLYMESH_MESH_H_