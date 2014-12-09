#ifndef _RT_LAMBERTIAN_H_
#define _RT_LAMBERTIAN_H_

#include "BRDF.h"

#include "utilities/Constants.h"

namespace rt
{

class Lambertian : public BRDF
{
public:

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

	void SetKa(const float k);

	void SetKd(const float k);

	void SetCd(const RGBColor c);

private:
	// diffuse reflection coefficient
	float		m_kd;
	// diffuse color
	RGBColor 	m_cd;

}; // Lambertian

inline RGBColor Lambertian::
f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	return m_kd * m_cd * INV_PI;
}

inline RGBColor Lambertian::
rho(const ShadeRec& sr, const Vector3D& wo) const {
	return m_kd * m_cd;
}

inline void Lambertian::
SetKa(const float k) {
	m_kd = k;
}

inline void Lambertian::SetKd(const float k) {
	m_kd = k;
}

inline void Lambertian::SetCd(const RGBColor c) {
	m_cd = c;
}

}

#endif // _RT_LAMBERTIAN_H_