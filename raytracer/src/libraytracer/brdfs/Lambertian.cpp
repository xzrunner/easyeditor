#include "Lambertian.h"

#include "texture/Texture.h"

namespace rt
{

RGBColor Lambertian::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	if (m_cd_tex)  {
		m_cd = m_cd_tex->GetColor(sr);
	}
	return m_kd * m_cd * (float)INV_PI;
}

RGBColor Lambertian::rho(const ShadeRec& sr, const Vector3D& wo) const 
{
	if (m_cd_tex)  {
		m_cd = m_cd_tex->GetColor(sr);
	}
	return m_kd * m_cd;
}

}