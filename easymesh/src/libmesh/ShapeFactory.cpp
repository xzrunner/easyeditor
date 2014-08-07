#include "ShapeFactory.h"
#include "Mesh.h"
#include "Strip.h"

namespace emesh
{

ShapeFactory* ShapeFactory::m_instance = NULL;

Shape* ShapeFactory::CreateShape(const d2d::Image& image) const
{
	Shape* shape = NULL;
	switch (m_type)
	{
	case ST_MESH:
		shape = new Mesh(image);
		break;
	case ST_STRIP:
		shape = new Strip(image);
		break;
	}
	return shape;
}

void ShapeFactory::SetShapeType(ShapeType type)
{
	m_type = type;
}

ShapeFactory* ShapeFactory::Instance()
{
	if (!m_instance)
	{
		m_instance = new ShapeFactory();
	}
	return m_instance;
}

}