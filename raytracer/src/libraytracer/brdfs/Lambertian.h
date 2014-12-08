#ifndef _RT_LAMBERTIAN_H_
#define _RT_LAMBERTIAN_H_

#include "BRDF.h"

namespace rt
{

class Lambertian : public BRDF
{
public:

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

private:
	// diffuse reflection coefficient
	float		m_kd;
	// diffuse color
	RGBColor 	m_cd;

}; // Lambertian

inline RGBColor Lambertian::
rho(const ShadeRec& sr, const Vector3D& wo) const {
	return m_kd * m_cd;
}


}

#endif // _RT_LAMBERTIAN_H_