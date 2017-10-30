#ifndef _EASYMESH_MESH_FACTORY_H_
#define _EASYMESH_MESH_FACTORY_H_

#include <ee/Symbol.h>

namespace emesh
{

class Mesh;

class MeshFactory
{
public:
	std::unique_ptr<Mesh> CreateMesh(const ee::SymPtr& base) const;

	void SetShapeType(int pm_mesh_type);

public:
	static MeshFactory* Instance();

private:
	MeshFactory() {}

private:
	int m_pm_mesh_type;

private:
	static MeshFactory* m_instance;

}; // MeshFactory

}

#endif // _EASYMESH_MESH_FACTORY_H_