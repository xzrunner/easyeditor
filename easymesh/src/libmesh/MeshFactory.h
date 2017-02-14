#ifndef _EASYMESH_MESH_FACTORY_H_
#define _EASYMESH_MESH_FACTORY_H_

namespace ee { class Symbol; }

namespace emesh
{

class Mesh;

class MeshFactory
{
public:
	Mesh* CreateMesh(const ee::Symbol* base) const;

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