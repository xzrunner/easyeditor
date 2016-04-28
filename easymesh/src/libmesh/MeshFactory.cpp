#include "MeshFactory.h"
#include "Network.h"
#include "Strip.h"

namespace emesh
{

MeshFactory* MeshFactory::m_instance = NULL;

Mesh* MeshFactory::CreateMesh(const ee::Symbol* base) const
{
	Mesh* mesh = NULL;
	switch (m_type)
	{
	case ST_NETWORK:
		mesh = new Network(base);
		break;
	case ST_STRIP:
		mesh = new Strip(base);
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