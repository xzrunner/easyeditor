#include "Sphere.h"

#include "maths/Ray.h"
#include "utilities/ShadeRec.h"

#include <math.h>

namespace rt
{

static const double kEpsilon = 0.001;

bool Sphere::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double 		t;
	Vector3D	temp 	= ray.ori - m_center;
	double 		a 		= ray.dir * ray.dir;
	double 		b 		= 2.0 * temp * ray.dir;
	double 		c 		= temp * temp - m_radius * m_radius;
	double 		disc	= b * b - 4.0 * a * c;

	if (disc < 0.0) {
		return false;
	} else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > kEpsilon) {
			tmin = t;
			sr.normal = (temp + t * ray.dir) / m_radius;
			sr.local_hit_point = ray.ori + t * ray.dir;
			return true;
		}

		t = (-b + e) / denom;    // larger root

		if (t > kEpsilon) {
			tmin = t;
			sr.normal = (temp + t * ray.dir) / m_radius;
			sr.local_hit_point = ray.ori + t * ray.dir;
			return true;
		}
	}

	return false;
}

}