#include "MultipleObjects.h"

#include "utilities/ShadeRec.h"
#include "world/World.h"

namespace rt
{

RGBColor MultipleObjects::TraceRay(const Ray& ray) const
{
	ShadeRec sr(m_world->HitObjects(ray));
	if (sr.hit_an_object) {
		return sr.color;
	} else {
		return m_world->GetBackgroundColor();
	}
}

}