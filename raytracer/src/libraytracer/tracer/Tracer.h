#ifndef _RT_TRACER_H_
#define _RT_TRACER_H_

#include "utilities/RGBColor.h"

namespace rt
{

class Ray;

class Tracer
{
public:
	
	virtual RGBColor TraceRay(const Ray& ray) const;

	virtual RGBColor TraceRay(const Ray& ray, int depth) const;

}; // Tracer

}

#endif // _RT_TRACER_H_