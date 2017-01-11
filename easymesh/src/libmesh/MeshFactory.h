#ifndef _EASYMESH_MESH_FACTORY_H_
#define _EASYMESH_MESH_FACTORY_H_

#include <sprite2/MeshType.h>

namespace ee { class Symbol; }

namespace emesh
{

class Mesh;

class MeshFactory
{
public:
	Mesh* CreateMesh(const ee::Symbol* base) const;

	void SetShapeType(s2::MeshType type);

public:
	static MeshFactory* Instance();

private:
	MeshFactory() {}

private:
	s2::MeshType m_type;

private:
	static MeshFactory* m_instance;

}; // MeshFactory

}

#endif // _EASYMESH_MESH_FACTORY_H_