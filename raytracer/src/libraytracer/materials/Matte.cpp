#include "Matte.h"

#include "maths/Vector3D.h"
#include "brdfs/Lambertian.h"
#include "world/World.h"
#include "lights/Light.h"
#include "utilities/ShadeRec.h"

namespace rt
{

RGBColor Matte::Shade(const ShadeRec& sr) const
{
	Vector3D 	wo 			= -sr.ray.dir;
	RGBColor 	L 			= m_ambient_brdf->rho(sr, wo) * sr.w.GetAmbient()->L(sr);
	const std::vector<Light*>& lights = sr.w.GetLights();

	for (int i = 0, n = lights.size(); i < n; i++) {
		Vector3D wi = lights[i]->GetDirection(sr);
		float ndotwi = float(sr.normal * wi);

		if (ndotwi > 0.0f) {
			bool in_shadow = false;
			if (lights[i]->CastsShadows()) {
				Ray shadowRay(sr.hit_point, wi);
				in_shadow = lights[i]->InShadow(shadowRay, sr);
			}
			if (!in_shadow) {
				L += m_diffuse_brdf->f(sr, wo, wi) * lights[i]->L(sr) * ndotwi;
			}
		}
	}

	return L;
}

}