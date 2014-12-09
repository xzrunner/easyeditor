#ifndef _RT_SV_MATTE_H_
#define _RT_SV_MATTE_H_

#include "Material.h"

namespace rt
{

class Lambertian;
class ShadeRec;
class Texture;

class SV_Matte : public Material
{
public:
	SV_Matte();
	virtual ~SV_Matte();

	virtual RGBColor Shade(const ShadeRec& sr) const;

	void SetKa(const float k);

	void SetKd(const float k);

	void SetCd(const Texture* tex);

private:
	Lambertian*	m_ambient_brdf;

	Lambertian*	m_diffuse_brdf;

}; // SV_Matte

}

#endif // _RT_SV_MATTE_H_