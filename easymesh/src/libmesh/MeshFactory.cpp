#include "MeshFactory.h"
#include "PointsMesh.h"
#include "StripMesh.h"
#include "SkeletonMesh.h"

#include <polymesh/MeshType.h>

namespace emesh
{

MeshFactory* MeshFactory::m_instance = NULL;

std::unique_ptr<Mesh> MeshFactory::CreateMesh(const ee::SymPtr& base) const
{
	std::unique_ptr<Mesh> mesh = nullptr;
	switch (m_pm_mesh_type)
	{
	case pm::MESH_POINTS:
		mesh = std::make_unique<PointsMesh>(base);
		break;
	case pm::MESH_STRIP:
//		mesh = new StripMesh(base);
		break;
	case pm::MESH_SKIN:
//		mesh = new SkinMesh(base);
		break;
	}
	return mesh;
}

void MeshFactory::SetShapeType(int pm_mesh_type)
{
	m_pm_mesh_type = pm_mesh_type;
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