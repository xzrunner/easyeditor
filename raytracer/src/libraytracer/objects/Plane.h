#ifndef _RT_PLANE_H_
#define _RT_PLANE_H_

#include "GeometricObject.h"

#include "maths/Point3D.h"
#include "maths/Normal.h"

namespace rt
{

class Plane : public GeometricObject
{
public:
	Plane();
	Plane(const Point3D& pos, const Normal& normal);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

private:
	Point3D m_pos;
	Normal m_normal;

}; // Plane

}

#endif // _RT_PLANE_H_