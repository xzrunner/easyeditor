#include "AABB.h"
#include "Ray.h"
#include "Point3D.h"

#include <float.h>

namespace rt
{

AABB::AABB()
{
	x0 = y0 = z0 = FLT_MAX;
	x1 = y1 = z1 = -FLT_MAX;
}

bool AABB::Hit(const Ray& ray) const
{
	double ox = ray.ori.x; double oy = ray.ori.y; double oz = ray.ori.z;
	double dx = ray.dir.x; double dy = ray.dir.y; double dz = ray.dir.z;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max; 

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0, t1;

	// find largest entering t value

	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;	

	// find smallest exiting t value

	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;

	if (tz_max < t1)
		t1 = tz_max;

	return t0 < t1 && t1 > FLT_EPSILON;
}

bool AABB::Inside(const Point3D& p) const
{
	return (p.x > x0 && p.x < x1) && (p.y > y0 && p.y < y1) && (p.z > z0 && p.z < z1);
}

}