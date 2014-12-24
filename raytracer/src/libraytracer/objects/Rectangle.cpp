#include "Rectangle.h"

#include "samplers/Sampler.h"
#include "maths/Ray.h"
#include "utilities/Constants.h"
#include "utilities/ShadeRec.h"

namespace rt
{

Rectangle::Rectangle(const Point3D& m_p0, const Vector3D& m_a, const Vector3D& m_b, const Normal& n)
	: m_p0(m_p0)
	, m_a(m_a)
	, m_b(m_b)
	, m_a_len_squared(m_a.LenSquared())
	, m_b_len_squared(m_b.LenSquared())
	, m_normal(n)
	, m_area((float)(m_a.Length() * m_b.Length()))
	, m_inv_area(1 / m_area)
	, m_sampler(NULL)
{
}

bool Rectangle::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double t = (m_p0 - ray.ori) * m_normal / (ray.dir * m_normal); 

	if (t <= EPSILON)
		return (false);

	Point3D p = ray.ori + t * ray.dir;
	Vector3D d = p - m_p0;

	double ddota = d * m_a;

	if (ddota < 0.0 || ddota > m_a_len_squared)
		return (false);

	double ddotb = d * m_b;

	if (ddotb < 0.0 || ddotb > m_b_len_squared)
		return (false);

	tmin 				= t;
	sr.normal 			= m_normal;
	sr.local_hit_point 	= p;

	return (true);
}

bool Rectangle::ShadowHit(const Ray& ray, float& tmin) const
{
	double t = (m_p0 - ray.ori) * m_normal / (ray.dir * m_normal); 

	if (t <= EPSILON)
		return (false);

	Point3D p = ray.ori + t * ray.dir;
	Vector3D d = p - m_p0;

	double ddota = d * m_a;

	if (ddota < 0.0 || ddota > m_a_len_squared)
		return (false);

	double ddotb = d * m_b;

	if (ddotb < 0.0 || ddotb > m_b_len_squared)
		return (false);

	tmin = (float)t;

	return (true);
}

Point3D Rectangle::Sample() const
{
	Point2D sample_point = m_sampler->SampleUnitSquare();
	return (m_p0 + sample_point.x * m_a + sample_point.y * m_b);
}

float Rectangle::Pdf(const ShadeRec& sr) const
{
	return m_inv_area;
}

Normal Rectangle::GetNormal(const Point3D& p) const
{
	return m_normal;
}

void Rectangle::SetSampler(Sampler* sampler)
{
	obj_assign((const Object*&)m_sampler, sampler);
}

}