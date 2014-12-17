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
	m_material->Release();
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
	obj_assign((const Object*&)m_material, m);
}

}