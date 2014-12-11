#include "SV_Matte.h"

#include "brdfs/Lambertian.h"
#include "maths/Vector3D.h"
#include "utilities/ShadeRec.h"
#include "world/World.h"
#include "lights/Light.h"

namespace rt
{

SV_Matte::SV_Matte()
	: m_ambient_brdf(new Lambertian)
	, m_diffuse_brdf(new Lambertian)
{
}

SV_Matte::~SV_Matte()
{
	delete m_ambient_brdf;
	delete m_diffuse_brdf;
}

// same with Matte::Shade
RGBColor SV_Matte::Shade(const ShadeRec& sr) const
{
	Vector3D wo = -sr.ray.dir;
	RGBColor L = m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
	const std::vector<Light*>& lights = sr.w.GetLights();
	for (int i = 0, n = lights.size(); i < n; i++) 
	{
		Vector3D wi = lights[i]->GetDirection(sr);
		float ndotwi = float(sr.normal * wi);

		if (ndotwi > 0.0f) {
			bool in_shadow = false;
			if (lights[i]->CastsShadows()) {
				Ray shadow_ray(sr.hit_point, wi);
				in_shadow = lights[i]->InShadow(shadow_ray, sr);
			}
			if (!in_shadow) {
				L += m_diffuse_brdf->f(sr, wo, wi) * lights[i]->L(sr) * ndotwi;
			}
		}
	}

	return L;
}

// same with Matte::Shade
RGBColor SV_Matte::AreaLightShade(const ShadeRec& sr) const
{
	Vector3D wo = -sr.ray.dir;
	RGBColor L = m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
	const std::vector<Light*>& lights = sr.w.GetLights();
	for (int i = 0, n = lights.size(); i < n; i++) 
	{
		Vector3D wi = lights[i]->GetDirection(sr);
		float ndotwi = float(sr.normal * wi);

		if (ndotwi > 0.0) {
			bool in_shadow = false;
			if (lights[i]->CastsShadows()) {
				Ray shadow_ray(sr.hit_point, wi);
				in_shadow = lights[i]->InShadow(shadow_ray, sr); 
			}
			if (!in_shadow) { 
				L += m_diffuse_brdf->f(sr, wo, wi) * lights[i]->L(sr) * lights[i]->G(sr) * ndotwi / lights[i]->Pdf(sr);
			}
		}
	}

	return L;
}

void SV_Matte::SetKa(const float k)
{
	m_ambient_brdf->SetKa(k);
}

void SV_Matte::SetKd(const float k)
{
	m_diffuse_brdf->SetKd(k);
}

void SV_Matte::SetCd(const Texture* tex)
{
	m_ambient_brdf->SetCd(tex);
	m_diffuse_brdf->SetCd(tex);
}

}