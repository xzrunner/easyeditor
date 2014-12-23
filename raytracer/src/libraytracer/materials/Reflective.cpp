#include "Reflective.h"

#include "maths/Vector3D.h"
#include "maths/Ray.h"
#include "brdfs/PerfectSpecular.h"
#include "world/World.h"
#include "tracer/Tracer.h"
#include "utilities/ShadeRec.h"

namespace rt
{

Reflective::Reflective()
{
	m_reflective_brdf = new PerfectSpecular();
}

Reflective::~Reflective()
{
	delete m_reflective_brdf;
}

RGBColor Reflective::Shade(const ShadeRec& sr) const
{
	RGBColor L(Phong::Shade(sr));  // direct illumination

	Vector3D wo = -sr.ray.dir;
	Vector3D wi;	
	RGBColor fr = m_reflective_brdf->sample_f(sr, wo, wi); 
	Ray reflected_ray(sr.hit_point, wi); 
	reflected_ray.depth = sr.depth + 1;

	L += fr * sr.w.GetTracer()->TraceRay(reflected_ray, sr.depth + 1) * (sr.normal * wi);

	return L;
}

RGBColor Reflective::AreaLightShade(const ShadeRec& sr) const
{
	return Shade(sr);
}

void Reflective::SetKr(const float k)
{
	m_reflective_brdf->SetKr(k);
}

void Reflective::SetCr(const RGBColor& c)
{
	m_reflective_brdf->SetCr(c);
}

}