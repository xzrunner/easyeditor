#ifndef _EASY3D_MATH3_H_
#define _EASY3D_MATH3_H_

namespace e3d
{

class AABB;
class Ray;

class Math3
{
public:
	static bool RayAABBIntersection(const AABB& aabb, const Ray& ray, sm::vec3* coord);

	static bool RayOBBIntersection(const AABB& aabb, const sm::vec3& pos, const sm::Quaternion& angle,
		const Ray& ray, sm::vec3* coord);

	static float GetDistance(const sm::vec3& a, const sm::vec3& b);
	static float GetDistanceSquare(const sm::vec3& a, const sm::vec3& b);

}; // Math3

}

#endif // _EASY3D_MATH3_H_