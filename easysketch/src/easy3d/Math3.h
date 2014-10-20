#ifndef _E3D_MATH3_H_
#define _E3D_MATH3_H_

#include "Vector.h"

namespace e3d
{

class Math3
{
public:
	bool HitBoundingBox(const vec3& min_box, const vec3& max_box, const vec3& origin,
		const vec3& dir, vec3* coord);

}; // Math3

}

#endif // _E3D_MATH3_H_