#ifndef _RT_WIREFRAME_BOX_H_
#define _RT_WIREFRAME_BOX_H_

#include "Compound.h"

namespace rt
{

class WireframeBox : public Compound
{
public:
	WireframeBox(const Point3D& p0, const Point3D& p1, float bevel_radius);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

private:
	AABB m_aabb;

	Point3D m_p0, m_p1;
	float m_br;

}; // WireframeBox

}

#endif // _RT_WIREFRAME_BOX_H_