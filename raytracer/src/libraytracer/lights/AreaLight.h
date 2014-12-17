#ifndef _RT_AREA_LIGHT_H_
#define _RT_AREA_LIGHT_H_

#include "Light.h"

#include "maths/Point3D.h"
#include "maths/Normal.h"
#include "maths/Vector3D.h"

namespace rt
{

class GeometricObject;
class Material;

class AreaLight : public Light
{
public:
	AreaLight();
	virtual ~AreaLight();

	virtual RGBColor L(const ShadeRec& sr) const;

	virtual Vector3D GetDirection(const ShadeRec& sr) const;

	virtual bool InShadow(const Ray& ray, const ShadeRec& sr) const;

	virtual float G(const ShadeRec& sr) const;
	virtual float Pdf(const ShadeRec& sr) const;

	void SetObject(GeometricObject* obj);
	void SetMaterial(Material* material);

private:
	GeometricObject* m_object;

	// will be an emissive material
	Material* m_material;

	mutable Point3D	m_sample_point;

	// assigned in get_direction - which therefore can't be const for any light
	mutable Normal m_light_normal;

	// unit direction from hit point being shaded to sample point on light surface
	mutable Vector3D m_wi;

}; // AreaLight

}

#endif // _RT_AREA_LIGHT_H_