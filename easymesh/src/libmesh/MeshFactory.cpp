#include "MeshFactory.h"
#include "Network.h"
#include "Strip.h"

namespace emesh
{

MeshFactory* MeshFactory::m_instance = NULL;

MeshFactory::MeshFactory() 
	: m_use_region(false) 
{
}

Mesh* MeshFactory::CreateMesh(const ee::Image& image) const
{
	Mesh* mesh = NULL;
	switch (m_type)
	{
	case ST_MESH:
		mesh = new Network(image, true, m_use_region);
		break;
	case ST_STRIP:
		mesh = new Strip(image);
		break;
	}
	return mesh;
}

void MeshFactory::SetShapeType(MeshType type)
{
	m_type = type;
}

void MeshFactory::SetUseRegion(bool use_region) 
{
	m_use_region = use_region;
}

bool MeshFactory::IsUseRegion() const
{
	return m_use_region;
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