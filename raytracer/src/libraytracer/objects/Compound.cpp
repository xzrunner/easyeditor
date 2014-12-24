#include "Compound.h"

#include "maths/Normal.h"
#include "utilities/ShadeRec.h"

#include <float.h>

namespace rt
{

Compound::~Compound()
{
	ClearObjects();
}

bool Compound::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double t; 
	Normal normal;
	Point3D local_hit_point;
	bool hit = false;
	tmin = FLT_MAX;
	int num_objects	= m_objects.size();
	for (int j = 0; j < num_objects; j++) {
		if (m_objects[j]->Hit(ray, t, sr) && (t < tmin)) {
			hit				= true;
			tmin 			= t;
			SetMaterial(m_objects[j]->GetMaterial());
			normal			= sr.normal;
			local_hit_point	= sr.local_hit_point;  
		}
	}

	if (hit) {
		sr.t				= tmin;
		sr.normal 			= normal;   
		sr.local_hit_point 	= local_hit_point;  
	}

	return hit;
}

bool Compound::ShadowHit(const Ray& ray, float& tmin) const
{
	float t; 
	Normal normal;
	Point3D	local_hit_point;
	bool hit = false;
	tmin = FLT_MAX;
	int num_objects	= m_objects.size();
	for (int j = 0; j < num_objects; j++) {
		if (m_objects[j]->ShadowHit(ray, t) && (t < tmin)) {
			hit				= true;
			tmin 			= t;
			SetMaterial(m_objects[j]->GetMaterial());
		}
	}

	return hit;
}

void Compound::AddObject(GeometricObject* obj)
{
	obj->Retain();
	m_objects.push_back(obj);
}

void Compound::ClearObjects()
{
	for (int i = 0, n = m_objects.size(); i < n; ++i) {
		m_objects[i]->Release();
	}
	m_objects.clear();
}

}