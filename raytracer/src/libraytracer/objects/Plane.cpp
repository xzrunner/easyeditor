#include "Plane.h"

#include "maths/Ray.h"
#include "utilities/ShadeRec.h"
#include "utilities/Constants.h"

namespace rt
{

Plane::Plane()
	: m_pos(0, 0, 0)
	, m_normal(0, 1, 0)
{
}

Plane::Plane(const Point3D& pos, const Normal& normal)
	: m_pos(pos)
	, m_normal(normal)
{
}

bool Plane::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	float t = (m_pos - ray.ori) * m_normal / (ray.dir * m_normal);

	if (t > EPSILON * 10) {
		tmin = t;
		sr.normal = m_normal;
		sr.local_hit_point = ray.ori + t * ray.dir;

		return true;
	}

	return false;
}

bool Plane::ShadowHit(const Ray& ray, float& tmin) const
{
	float t = (m_pos - ray.ori) * m_normal / (ray.dir * m_normal);

	if (t > EPSILON * 10) {
		tmin = t;
		return (true);
	} else {
		return (false);
	}
}

}