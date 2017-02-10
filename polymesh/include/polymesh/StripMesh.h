#ifndef _POLYMESH_STRIP_MESH_H_
#define _POLYMESH_STRIP_MESH_H_

#include "Mesh.h"

namespace pm
{

class StripMesh : public Mesh
{
public:
	StripMesh() {}

	virtual MeshType Type() const { return MESH_STRIP; }
	
private:

}; // StripMesh

}

#endif // _POLYMESH_STRIP_MESH_H_