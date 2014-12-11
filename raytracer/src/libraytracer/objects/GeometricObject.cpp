#include "GeometricObject.h"

#include "materials/Material.h"
#include "maths/Normal.h"

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

Point3D GeometricObject::Sample() const
{
	return Point3D(0, 0, 0);
}

float GeometricObject::Pdf(const ShadeRec& sr) const
{
	return 0;
}

Normal GeometricObject::GetNormal(void) const
{
	return Normal();
}

Normal GeometricObject::GetNormal(const Point3D& p) const
{
	return Normal();
}

void GeometricObject::SetMaterial(const Material* m) 
{
	if (m_material != m) {
		delete m_material;
		m_material = m;
	}
}

}