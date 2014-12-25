#include "Triangle.h"

#include "maths/Ray.h"
#include "utilities/ShadeRec.h"
#include "utilities/Constants.h"

#include <float.h>
#include <math.h>

namespace rt
{

Triangle::Triangle()
	: v0(0, 0, 0)
	, v1(0,0,1)
	, v2(1,0,0)
	, normal(0, 1, 0)
{	
}

Triangle::Triangle(const Point3D& v0, const Point3D& v1, const Point3D& v2)
	: v0(v0)
	, v1(v1)
	, v2(v2)
	, normal(0, 1, 0)
{
}

bool Triangle::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.dir.x, d = v0.x - ray.ori.x; 
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.dir.y, h = v0.y - ray.ori.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.dir.z, l = v0.z - ray.ori.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom  = 1.0 / (a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
		return (false);

	double r = r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0 )
		return (false);

	if (beta + gamma > 1.0)
		return (false);

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < EPSILON) 
		return (false);

	tmin 				= t;
	sr.normal 			= normal;  	
	sr.local_hit_point 	= ray.ori + t * ray.dir;	

	return (true);	
}

bool Triangle::ShadowHit(const Ray& ray, float& tmin) const
{
	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.dir.x, d = v0.x - ray.ori.x; 
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.dir.y, h = v0.y - ray.ori.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.dir.z, l = v0.z - ray.ori.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom  = 1.0 / (a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
		return (false);

	double r = r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0 )
		return (false);

	if (beta + gamma > 1.0)
		return (false);

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < EPSILON) 
		return (false);

	tmin 				= t;

	return (true);	
}

AABB Triangle::GetBoundingBox() const
{
	double delta = 0.000001; 
	return (AABB(std::min(std::min(v0.x, v1.x), v2.x) - delta, std::max(std::max(v0.x, v1.x), v2.x) + delta, 
				 std::min(std::min(v0.y, v1.y), v2.y) - delta, std::max(std::max(v0.y, v1.y), v2.y) + delta, 
				 std::min(std::min(v0.z, v1.z), v2.z) - delta, std::max(std::max(v0.z, v1.z), v2.z) + delta));
}

}