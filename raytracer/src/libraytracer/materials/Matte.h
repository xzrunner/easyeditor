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
	Matte();
	virtual ~Matte();

	virtual RGBColor Shade(const ShadeRec& sr) const;
	virtual RGBColor AreaLightShade(const ShadeRec& sr) const;

	void SetKa(const float k);
	void SetKd(const float k);
	void SetCd(const RGBColor c);

private:
	Lambertian*	m_ambient_brdf;
	Lambertian*	m_diffuse_brdf;

}; // Matte

}

#endif // _RT_MATTE_H_