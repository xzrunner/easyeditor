#include "OpenCylinder.h"

#include "maths/Ray.h"
#include "utilities/Constants.h"
#include "utilities/ShadeRec.h"

namespace rt
{

OpenCylinder::OpenCylinder(const OpenCylinder& cy)
	: m_y0(cy.m_y0)
	, m_y1(cy.m_y1)
	, m_radius(cy.m_radius)
	, m_inv_radius(1 / m_inv_radius)
{	
}

OpenCylinder::OpenCylinder(double bottom, double top, double radius)
	: m_y0(bottom)
	, m_y1(top)
	, m_radius(radius)
	, m_inv_radius(1 / radius)
{
}

OpenCylinder* OpenCylinder::Clone() const
{
	return new OpenCylinder(*this);
}

bool OpenCylinder::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double t;
	double ox = ray.ori.x;
	double oy = ray.ori.y;
	double oz = ray.ori.z;
	double dx = ray.dir.x;
	double dy = ray.dir.y;
	double dz = ray.dir.z;

	double a = dx * dx + dz * dz;  	
	double b = 2.0 * (ox * dx + oz * dz);					
	double c = ox * ox + oz * oz - m_radius * m_radius;
	double disc = b * b - 4.0 * a * c ;


	if (disc < 0.0)
		return(false);
	else {	
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > EPSILON) {
			double yhit = oy + t * dy;

			if (yhit > m_y0 && yhit < m_y1) {
				tmin = t;
				sr.normal = Normal((ox + t * dx) * m_inv_radius, 0.0, (oz + t * dz) * m_inv_radius);

				// test for hitting from inside

				if (-ray.dir * sr.normal < 0.0)
					sr.normal = -sr.normal;

				sr.local_hit_point = ray.ori + tmin * ray.dir;

				return (true);
			}
		} 

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {
			double yhit = oy + t * dy;

			if (yhit > m_y0 && yhit < m_y1) {
				tmin = t;
				sr.normal = Normal((ox + t * dx) * m_inv_radius, 0.0, (oz + t * dz) * m_inv_radius);

				// test for hitting inside surface

				if (-ray.dir * sr.normal < 0.0)
					sr.normal = -sr.normal;

				sr.local_hit_point = ray.ori + tmin * ray.dir;

				return (true);
			}
		} 
	}

	return (false);			
}

bool OpenCylinder::ShadowHit(const Ray& ray, float& tmin) const
{
	double t;
	double ox = ray.ori.x;
	double oy = ray.ori.y;
	double oz = ray.ori.z;
	double dx = ray.dir.x;
	double dy = ray.dir.y;
	double dz = ray.dir.z;

	double a = dx * dx + dz * dz;  	
	double b = 2.0 * (ox * dx + oz * dz);					
	double c = ox * ox + oz * oz - m_radius * m_radius;
	double disc = b * b - 4.0 * a * c ;


	if (disc < 0.0)
		return(false);
	else {	
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > EPSILON) {
			double yhit = oy + t * dy;

			if (yhit > m_y0 && yhit < m_y1) {
				tmin = t;

				return (true);
			}
		} 

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {
			double yhit = oy + t * dy;

			if (yhit > m_y0 && yhit < m_y1) {
				tmin = t;

				return (true);
			}
		} 
	}

	return (false);			
}

}