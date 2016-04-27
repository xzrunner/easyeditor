#include "MeshFactory.h"
#include "Network.h"
#include "Strip.h"

namespace emesh
{

MeshFactory* MeshFactory::m_instance = NULL;

Mesh* MeshFactory::CreateMesh(const ee::Image& image) const
{
	Mesh* mesh = NULL;
	switch (m_type)
	{
	case ST_NETWORK:
		mesh = new Network(image);
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

MeshFactory* MeshFactory::Instance()
{
	if (!m_instance)
	{
		m_instance = new MeshFactory();
	}
	return m_instance;
}

}