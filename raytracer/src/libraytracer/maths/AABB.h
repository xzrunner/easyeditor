#ifndef _RT_AABB_H_
#define _RT_AABB_H_

namespace rt
{

class Ray;
class Point3D;

class AABB
{
public:
	AABB();
	AABB(float x0, float x1, float y0, float y1, float z0, float z1);

	bool Hit(const Ray& ray) const;

	bool Inside(const Point3D& p) const;

public:
	double x0, x1, y0, y1, z0, z1;

}; // AABB

}

#endif // _RT_AABB_H_