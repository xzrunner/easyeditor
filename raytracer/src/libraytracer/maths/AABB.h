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

	bool Hit(const Ray& ray) const;

	bool Inside(const Point3D& p) const;

public:
	double x0, x1, y0, y1, z0, z1;

}; // AABB

}

#endif // _RT_AABB_H_