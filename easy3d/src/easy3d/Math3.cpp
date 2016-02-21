#include "Math3.h"
#include "AABB.h"
#include "Ray.h"

namespace e3d
{

#define NUMDIM	3
#define RIGHT	0
#define LEFT	1
#define MIDDLE	2

bool Math3::RayAABBIntersection(const AABB& aabb, const Ray& ray, ee::vec3* coord)
{
	ee::vec3 cross;

	char quadrant[3];
	float candidate_plane[3];
	bool inside = true;

	/* Find candidate planes; this loop can be avoided if
   	rays cast all from the eye(assume perpsective view) */
	for (int i = 0; i < 3; ++i)
	{
		if (ray.Start()[i] < aabb.Min()[i]) {
			quadrant[i] = LEFT;
			candidate_plane[i] = aabb.Min()[i];
			inside = false;
		} else if (ray.Start()[i] > aabb.Max()[i]) {
			quadrant[i] = RIGHT;
			candidate_plane[i] = aabb.Max()[i];
			inside = false;
		} else {
			quadrant[i] = MIDDLE;
		}
	}

	/* Ray origin inside bounding box */
	if (inside) {
		cross = ray.Start();
		return true;
	}

	float max_t[3];
	/* Calculate T distances to candidate planes */
	for (int i = 0; i < 3; ++i) {
		if (quadrant[i] != MIDDLE && ray.Dir()[i] != 0) {
			max_t[i] = (candidate_plane[i]-ray.Start()[i])/ray.Dir()[i];
		} else {
			max_t[i] = -1;
		}
	}

	/* Get largest of the maxT's for final choice of intersection */
	int which_plane = 0;
	for (int i = 1; i < 3; ++i) {
		if (max_t[which_plane] < max_t[i]) {
			which_plane = i;
		}
	}

	/* Check final candidate actually inside box */
	if (max_t[which_plane] < 0) {
		return false;
	}
	for (int i = 0; i < 3; ++i) {
		if (which_plane != i) {
			cross[i] = ray.Start()[i] + max_t[which_plane] * ray.Dir()[i];
			if (cross[i] < aabb.Min()[i] || cross[i] > aabb.Max()[i]) {
				return false;
			}
		} else {
			cross[i] = candidate_plane[i];
		}
	}

	if (coord) {
		*coord = cross;
	}

	return true;
}

bool Math3::RayOBBIntersection(const AABB& aabb, const ee::vec3& pos, const ee::Quaternion& angle, 
							   const Ray& ray, ee::vec3* coord)
{
	Ray _ray(ray);
	_ray.Translate(-pos);
	_ray.ChangeCoordSystem(angle);

	return RayAABBIntersection(aabb, _ray, coord);
}

float Math3::GetDistance(const ee::vec3& a, const ee::vec3& b)
{
	return sqrt(GetDistanceSquare(a, b));
}

float Math3::GetDistanceSquare(const ee::vec3& a, const ee::vec3& b)
{
	float dx = a.x - b.x,
		dy = a.y - b.y,
		dz = a.z - b.z;
	return dx*dx+dy*dy+dz*dz;
}

}