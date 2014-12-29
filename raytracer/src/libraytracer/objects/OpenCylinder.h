#ifndef _RT_OPEN_CYLINDER_H_
#define _RT_OPEN_CYLINDER_H_

#include "GeometricObject.h"

namespace rt
{

class OpenCylinder : public GeometricObject
{
public:	
	OpenCylinder(const OpenCylinder& cy);
	OpenCylinder(double bottom, double top, double radius);

	virtual OpenCylinder* Clone() const;

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

private:
	// bottom y value
	double		m_y0;		
	// top y value
	double		m_y1;				

	double		m_radius;
	double		m_inv_radius;

}; // OpenCylinder

}

#endif // _RT_OPEN_CYLINDER_H_