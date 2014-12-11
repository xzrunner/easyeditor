#ifndef _RT_TRACER_H_
#define _RT_TRACER_H_

#include "utilities/RGBColor.h"

namespace rt
{

class World;
class Ray;

class Tracer
{
public:
	Tracer(const World* world);
	virtual ~Tracer() {}

	virtual RGBColor TraceRay(const Ray& ray) const;

	virtual RGBColor TraceRay(const Ray& ray, int depth) const;

protected:
	const World* m_world;

}; // Tracer

}

#endif // _RT_TRACER_H_