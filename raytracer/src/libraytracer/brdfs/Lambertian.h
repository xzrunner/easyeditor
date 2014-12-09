#ifndef _RT_LAMBERTIAN_H_
#define _RT_LAMBERTIAN_H_

#include "BRDF.h"

#include "utilities/Constants.h"

namespace rt
{

class Texture;

class Lambertian : public BRDF
{
public:

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

	void SetKa(const float k);

	void SetKd(const float k);

	void SetCd(const RGBColor c);
	void SetCd(const Texture* tex);

private:
	// diffuse reflection coefficient
	float m_kd;
	// diffuse color
	mutable RGBColor m_cd;
	const Texture* m_cd_tex;

}; // Lambertian

inline void Lambertian::SetKa(const float k) {
	m_kd = k;
}

inline void Lambertian::SetKd(const float k) {
	m_kd = k;
}

inline void Lambertian::SetCd(const RGBColor c) {
	m_cd = c;
}

inline void Lambertian::SetCd(const Texture* tex) {
	m_cd_tex = tex;
}

}

#endif // _RT_LAMBERTIAN_H_