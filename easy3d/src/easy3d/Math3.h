#ifndef _E3D_MATH3_H_
#define _E3D_MATH3_H_

#include "Vector.h"
#include "Quaternion.h"

namespace e3d
{

class AABB;
class Ray;

class Math3
{
public:
	static bool RayAABBIntersection(const AABB& aabb, const Ray& ray, vec3* coord);

	static bool RayOBBIntersection(const AABB& aabb, const vec3& pos, const Quaternion& angle,
		const Ray& ray, vec3* coord);

	static float GetDistance(const vec3& a, const vec3& b);

}; // Math3

}

#endif // _E3D_MATH3_H_