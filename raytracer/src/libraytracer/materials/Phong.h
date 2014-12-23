#ifndef _RT_PHONG_H_
#define _RT_PHONG_H_

#include "Material.h"

namespace rt
{

class Lambertian;
class GlossySpecular;

class Phong : public Material
{
public:
	Phong();
	virtual ~Phong();

	virtual RGBColor Shade(const ShadeRec& sr) const;

	void SetKa(const float k);
	void SetKd(const float k);
	void SetKs(const float ks);
	void SetCd(const RGBColor& c);
	void SetCs(const RGBColor& c);
	void SetExp(const float e);

private:
	Lambertian*		m_ambient_brdf;
	Lambertian*		m_diffuse_brdf;
	GlossySpecular*	m_specular_brdf;

}; // Phong

}

#endif // _RT_PHONG_H_