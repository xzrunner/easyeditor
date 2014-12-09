#ifndef _RT_SPHERE_H_
#define _RT_SPHERE_H_

#include "GeometricObject.h"

#include "maths/Point3D.h"

namespace rt
{

class Sphere : public GeometricObject
{
public:
	Sphere(const Point3D& center, float r);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

private:
	Point3D m_center;
	float m_radius;

}; // Sphere

}

#endif // _RT_SPHERE_H_