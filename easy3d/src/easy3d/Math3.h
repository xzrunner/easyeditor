#ifndef _EASY3D_MATH3_H_
#define _EASY3D_MATH3_H_

#include <ee/Vector3D.h>
#include <ee/Quaternion.h>

namespace e3d
{

class AABB;
class Ray;

class Math3
{
public:
	static bool RayAABBIntersection(const AABB& aabb, const Ray& ray, ee::vec3* coord);

	static bool RayOBBIntersection(const AABB& aabb, const ee::vec3& pos, const ee::Quaternion& angle,
		const Ray& ray, ee::vec3* coord);

	static float GetDistance(const ee::vec3& a, const ee::vec3& b);
	static float GetDistanceSquare(const ee::vec3& a, const ee::vec3& b);

}; // Math3

}

#endif // _EASY3D_MATH3_H_