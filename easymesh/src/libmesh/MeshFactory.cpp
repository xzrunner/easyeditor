#include "MeshFactory.h"
#include "Network.h"
#include "Strip.h"
#include "Skeleton.h"

namespace emesh
{

MeshFactory* MeshFactory::m_instance = NULL;

Mesh* MeshFactory::CreateMesh(const ee::Symbol* base) const
{
	Mesh* mesh = NULL;
	switch (m_type)
	{
	case s2::MESH_NETWORK:
		mesh = new Network(base);
		break;
	case s2::MESH_STRIP:
		mesh = new Strip(base);
		break;
	case s2::MESH_SKELETON:
		mesh = new Skeleton(base);
		break;
	}
	return mesh;
}

void MeshFactory::SetShapeType(pm::MeshType type)
{
	m_type = type;
}

MeshFactory* MeshFactory::Instance()
{
	if (!m_instance)
	{
		m_instance = new MeshFactory();
	}
	return m_instance;
}

}