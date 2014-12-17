#include "AreaLighting.h"

#include "utilities/ShadeRec.h"
#include "world/World.h"
#include "materials/Material.h"

namespace rt
{

AreaLighting::AreaLighting(const World* world)
	: Tracer(world)
{
}

RGBColor AreaLighting::TraceRay(const Ray& ray) const
{
	ShadeRec sr(m_world->HitObjects(ray));

	if (sr.hit_an_object) {
		sr.ray = ray;
		return sr.material->AreaLightShade(sr);
	} else {
		return m_world->GetBackgroundColor();
	}
}

// this ignores the depth argument
RGBColor AreaLighting::TraceRay(const Ray& ray, int depth) const
{
	return TraceRay(ray);
}

}