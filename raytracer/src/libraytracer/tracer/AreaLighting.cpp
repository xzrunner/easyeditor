#include "AreaLighting.h"

namespace rt
{

RGBColor AreaLighting::TraceRay(const Ray& ray) const
{
	ShadeRec sr(m_world->HitObjects(ray));

	if (sr.hit_an_object) {
		sr.ray = ray;
		return sr.material->area_light_shade(sr);
	} else {
		return m_world->GetBackgroundColor();
	}
}

// this ignores the depth argument
RGBColor AreaLighting::TraceRay(const Ray& ray, int depth) const
{
	TraceRay(ray);
}

}