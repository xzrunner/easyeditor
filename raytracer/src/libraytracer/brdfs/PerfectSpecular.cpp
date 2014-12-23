#include "PerfectSpecular.h"

#include "utilities/ShadeRec.h"

namespace rt
{

PerfectSpecular::PerfectSpecular()
	: m_kr(0)
	, m_cr(1, 1, 1)
{
}

RGBColor PerfectSpecular::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	//return (kr * cr * invPI);
	return BRDF::f(sr, wi, wo);
}

RGBColor PerfectSpecular::rho(const ShadeRec& sr, const Vector3D& wo) const
{
	//return (kr * cr);
	return BRDF::rho(sr, wo);
}

// ---------------------------------------------------------- sample_f
// this computes wi: the direction of perfect mirror reflection
// it's called from from the functions Reflective::shade and Transparent::shade.
// the fabs in the last statement is for transparency
// explained on page 500
RGBColor PerfectSpecular::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const
{
	float ndotwo = sr.normal * wo;
	wi = -wo + 2.0 * sr.normal * ndotwo;
	return (m_kr * m_cr / fabs(sr.normal * wi)); // why is this fabs? // kr would be a Fresnel term in a Fresnel reflector

}

// ---------------------------------------------------------- sample_f
// this version of sample_f is used with path tracing
// it returns ndotwi in the pdf
RGBColor PerfectSpecular::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
	float ndotwo = sr.normal * wo;
	wi = -wo + 2.0 * sr.normal * ndotwo;
	pdf = fabs(sr.normal * wi);
	return (m_kr * m_cr);
}

void PerfectSpecular::SetKr(const float k)
{
	m_kr = k;
}

void PerfectSpecular::SetCr(const RGBColor& c)
{
	m_cr = c;
}

}