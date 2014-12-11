#include "AreaLight.h"

#include "materials/Material.h"
#include "objects/GeometricObject.h"
#include "world/World.h"
#include "utilities/Constants.h"
#include "utilities/ShadeRec.h"

namespace rt
{

AreaLight::AreaLight()
	: m_object(NULL)
	, m_material(NULL)
{
}

AreaLight::~AreaLight()
{
	delete m_object;
	delete m_material;
}

RGBColor AreaLight::L(const ShadeRec& sr) const
{
	float ndotd = -m_light_normal * m_wi;
	if (ndotd > 0.0f) {
		return m_material->GetLe(sr);
	} else {
		return BLACK;
	}
}

Vector3D AreaLight::GetDirection(const ShadeRec& sr) const
{
	m_sample_point = m_object->Sample();	// used in the G function
	m_light_normal = m_object->GetNormal(m_sample_point); 
	m_wi = m_sample_point - sr.hit_point;		// used in the G function
	m_wi.Normalize();

	return m_wi;
}

bool AreaLight::InShadow(const Ray& ray, const ShadeRec& sr) const
{
	float t;
	float ts = (m_sample_point - ray.ori) * ray.dir;
	const std::vector<GeometricObject*>& objs = sr.w.GetObjects();
	for (int i=  0, n = objs.size(); i < n; ++i) {
		if (objs[i]->ShadowHit(ray, t) && t < ts) {
			return true; 
		}
	}

	return false;
}

float AreaLight::G(const ShadeRec& sr) const
{
	float ndotd = -m_light_normal * m_wi;
	float d2 	= m_sample_point.DistanceSquared(sr.hit_point);

	return (ndotd / d2);
}

float AreaLight::Pdf(const ShadeRec& sr) const
{
	return (m_object->Pdf(sr));
}

void AreaLight::SetObject(GeometricObject* obj)
{
	if (m_object != obj) {
		delete m_object;
		m_object = obj;
	}
}

}