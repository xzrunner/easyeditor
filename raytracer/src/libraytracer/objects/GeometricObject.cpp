#include "GeometricObject.h"

#include "materials/Material.h"

namespace rt
{

GeometricObject::GeometricObject()
	: m_material(NULL)
{
}

GeometricObject::~GeometricObject()
{
	delete m_material;
}

void GeometricObject::SetMaterial(const Material* m) 
{
	if (m_material != m) {
		delete m_material;
		m_material = m;
	}
}

}