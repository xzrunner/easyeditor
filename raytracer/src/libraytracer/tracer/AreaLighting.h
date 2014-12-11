#ifndef _RT_AREA_LIGHTING_H_
#define _RT_AREA_LIGHTING_H_

#include "Tracer.h"

namespace rt
{

class AreaLighting : public Tracer
{
public:
	virtual RGBColor TraceRay(const Ray& ray) const;

	virtual RGBColor TraceRay(const Ray& ray, int depth) const;

}; // AreaLighting

}

#endif // _RT_AREA_LIGHTING_H_