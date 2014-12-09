#ifndef _RT_POINT_LIGHT_H_
#define _RT_POINT_LIGHT_H_

#include "Light.h"

#include "maths/Point3D.h"

namespace rt
{

class PointLight : public Light
{
public:
	PointLight();

	virtual RGBColor L(const ShadeRec& sr) const;

	virtual Vector3D GetDirection(const ShadeRec& sr) const;

	virtual bool InShadow(const Ray& ray, const ShadeRec& sr) const;

	void SetLocation(const Point3D& location);

	void ScaleRadiance(const float b);

private:
	// radiant scale factor
	float m_ls;
	RGBColor m_color;

	Point3D m_location;

}; // PointLight

}

#endif // _RT_POINT_LIGHT_H_