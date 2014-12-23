#ifndef _RT_PERFECT_SPECULAR_H_
#define _RT_PERFECT_SPECULAR_H_

#include "BRDF.h"

namespace rt
{

class PerfectSpecular : public BRDF
{
public:
	PerfectSpecular();
	
	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

	void SetKr(const float k);
	void SetCr(const RGBColor& c);

private:
	// reflection coefficient
	float m_kr;	

	// the reflection color
	RGBColor m_cr;

}; // PerfectSpecular

}

#endif // _RT_PERFECT_SPECULAR_H_