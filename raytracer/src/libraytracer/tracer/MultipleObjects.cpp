#include "MultipleObjects.h"

#include "utilities/ShadeRec.h"
#include "world/World.h"

namespace rt
{

MultipleObjects::MultipleObjects(const World* world)
	: Tracer(world)
{	
}

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