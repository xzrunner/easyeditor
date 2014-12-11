#ifndef _RT_MULTIPLE_OBJECTS_H_
#define _RT_MULTIPLE_OBJECTS_H_

#include "Tracer.h"

namespace rt
{

class World;

class MultipleObjects : public Tracer
{
public:	
	virtual RGBColor TraceRay(const Ray& ray) const;

}; // MultipleObjects

}

#endif // _RT_MULTIPLE_OBJECTS_H_