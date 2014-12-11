#include "PointLight.h"

#include "utilities/ShadeRec.h"
#include "objects/GeometricObject.h"
#include "world/World.h"

#include <vector>

namespace rt
{

PointLight::PointLight()
	: m_ls(1)
	, m_color(1, 1, 1)
{
}

RGBColor PointLight::L(const ShadeRec& sr) const
{
	return m_ls * m_color;
}

Vector3D PointLight::GetDirection(const ShadeRec& sr) const
{
	Vector3D dir = m_location - sr.hit_point;
	dir.Normalize();
	return dir;
}

bool PointLight::InShadow(const Ray& ray, const ShadeRec& sr) const
{
	float t;
	float d = m_location.Distance(ray.ori);

	const std::vector<GeometricObject*>& objs = sr.w.GetObjects();
	for (int i=  0, n = objs.size(); i < n; ++i) {
		if (objs[i]->ShadowHit(ray, t) && t < d)
			return true;
	}

	return false;
}

float PointLight::G(const ShadeRec& sr) const
{
	return 5.5f;
}

float PointLight::Pdf(const ShadeRec& sr) const
{
	return 5.5f;
}

void PointLight::SetLocation(const Point3D& location)
{
	m_location = location;
}

void PointLight::ScaleRadiance(const float b)
{
	m_ls *= b;
}

}