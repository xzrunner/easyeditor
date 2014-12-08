#ifndef _RT_MATTE_H_
#define _RT_MATTE_H_

#include "Material.h"

namespace rt
{

class Lambertian;
class ShadeRec;
class Vector3D;

class Matte : public Material
{
public:

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const = 0;

	virtual RGBColor Shade(const ShadeRec& sr) const;

private:
	Lambertian*	m_ambient_brdf;
	Lambertian*	m_diffuse_brdf;

}; // Matte

}

#endif // _RT_MATTE_H_