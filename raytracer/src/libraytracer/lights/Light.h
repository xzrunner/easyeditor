#ifndef _RT_LIGHT_H_
#define _RT_LIGHT_H_

#include "maths/Vector3D.h"
#include "utilities/RGBColor.h"

namespace rt
{

class ShadeRec;
class Ray;

class Light
{
public:
	Light();

	virtual RGBColor L(const ShadeRec& sr) const = 0;

	virtual Vector3D GetDirection(const ShadeRec& sr) const = 0;

	virtual bool CastsShadows() const;

	virtual bool InShadow(const Ray& ray, const ShadeRec& sr) const = 0;

	virtual float G(const ShadeRec& sr) const = 0;
	virtual float Pdf(const ShadeRec& sr) const = 0;

	void SetShadows(bool shadows) { m_shadows = shadows; }

private:
	// does the light cast shadows or not
	bool m_shadows;

}; // Light

}

#endif // _RT_LIGHT_H_