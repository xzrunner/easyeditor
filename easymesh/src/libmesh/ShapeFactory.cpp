#include "ShapeFactory.h"
#include "Mesh.h"
#include "Strip.h"

namespace emesh
{

ShapeFactory* ShapeFactory::m_instance = NULL;

ShapeFactory::ShapeFactory() 
	: m_use_region(false) 
{
}

Shape* ShapeFactory::CreateShape(const ee::Image& image) const
{
	Shape* shape = NULL;
	switch (m_type)
	{
	case ST_MESH:
		shape = new Mesh(image, true, m_use_region);
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

void ShapeFactory::SetUseRegion(bool use_region) 
{
	m_use_region = use_region;
}

bool ShapeFactory::IsUseRegion() const
{
	return m_use_region;
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