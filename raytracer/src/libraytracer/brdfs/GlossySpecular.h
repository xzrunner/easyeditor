#ifndef _RT_GLOSSY_SPECULAR_H_
#define _RT_GLOSSY_SPECULAR_H_

#include "BRDF.h"

namespace rt
{

class Sampler;

class GlossySpecular : public BRDF
{
public:
	GlossySpecular();
	virtual ~GlossySpecular();

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

	void SetKs(const float ks);
	void SetExp(const float e);
	void SetCs(const RGBColor& c);

private:
	// specular reflection coefficient
	float m_ks;

	// specular exponent
	float m_exp;

	// specular color
	RGBColor m_cs;

	// for use in sample_f
	Sampler* m_sampler;

}; // GlossySpecular

}

#endif // _RT_GLOSSY_SPECULAR_H_