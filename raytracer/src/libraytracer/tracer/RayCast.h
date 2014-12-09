#ifndef _RT_RAY_CAST_H_
#define _RT_RAY_CAST_H_

#include "Tracer.h"

namespace rt
{

class World;

class RayCast : public Tracer
{
public:
	RayCast(const World* world);

	virtual RGBColor TraceRay(const Ray& ray) const;

	virtual RGBColor TraceRay(const Ray& ray, int depth) const;

private:
	const World* m_world;

}; // RayCast

}

#endif // _RT_RAY_CAST_H_