#include "Box.h"

#include "maths/Ray.h"
#include "maths/Normal.h"
#include "utilities/ShadeRec.h"
#include "utilities/Constants.h"

namespace rt
{

Box::Box(const Point3D& min, const Point3D& max)
{
	x0 = min.x;
	y0 = min.y;
	z0 = min.z;

	x1 = max.x;
	y1 = max.y;
	z1 = max.z;
}

bool Box::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double ox = ray.ori.x; double oy = ray.ori.y; double oz = ray.ori.z;
	double dx = ray.dir.x; double dy = ray.dir.y; double dz = ray.dir.z;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	} else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	} else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	} else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0, t1;

	int face_in, face_out;

	// find largest entering t value

	if (tx_min > ty_min) {
		t0 = tx_min;
		face_in = (a >= 0.0) ? 0 : 3;
	} else {
		t0 = ty_min;
		face_in = (b >= 0.0) ? 1 : 4;
	}

	if (tz_min > t0) {
		t0 = tz_min;
		face_in = (c >= 0.0) ? 2 : 5;
	}

	// find smallest exiting t value

	if (tx_max < ty_max) {
		t1 = tx_max;
		face_out = (a >= 0.0) ? 3 : 0;
	} else {
		t1 = ty_max;
		face_out = (b >= 0.0) ? 4 : 1;
	}

	if (tz_max < t1) {
		t1 = tz_max;
		face_out = (c >= 0.0) ? 5 : 2;
	}

	if (t0 < t1 && t1 > EPSILON) {  // condition for a hit
		if (t0 > EPSILON) {
			tmin = t0;  			// ray hits outside surface
			sr.normal = GetNormal(face_in);
		}
		else {
			tmin = t1;				// ray hits inside surface
			sr.normal = GetNormal(face_out);
		}

		sr.local_hit_point = ray.ori + tmin * ray.dir;
		return (true);
	} else {
		return (false);	
	}
}

bool Box::ShadowHit(const Ray& ray, float& tmin) const
{
	double ox = ray.ori.x; double oy = ray.ori.y; double oz = ray.ori.z;
	double dx = ray.dir.x; double dy = ray.dir.y; double dz = ray.dir.z;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	} else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	} else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	} else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0, t1;

	int face_in, face_out;

	// find largest entering t value

	if (tx_min > ty_min) {
		t0 = tx_min;
		face_in = (a >= 0.0) ? 0 : 3;
	} else {
		t0 = ty_min;
		face_in = (b >= 0.0) ? 1 : 4;
	}

	if (tz_min > t0) {
		t0 = tz_min;
		face_in = (c >= 0.0) ? 2 : 5;
	}

	// find smallest exiting t value

	if (tx_max < ty_max) {
		t1 = tx_max;
		face_out = (a >= 0.0) ? 3 : 0;
	} else {
		t1 = ty_max;
		face_out = (b >= 0.0) ? 4 : 1;
	}

	if (tz_max < t1) {
		t1 = tz_max;
		face_out = (c >= 0.0) ? 5 : 2;
	}

	if (t0 < t1 && t1 > EPSILON) {  // condition for a hit
		if (t0 > EPSILON)
			tmin = t0;  			// ray hits outside surface
		else 
			tmin = t1;				// ray hits inside surface

		return (true);
	} else {
		return (false);	
	}
}

AABB Box::GetBoundingBox() const
{
	AABB aabb;
	aabb.x0 = x0;
	aabb.y0 = y0;
	aabb.z0 = z0;
	aabb.x1 = x1;
	aabb.y1 = y1;
	aabb.z1 = z1;
	return aabb;
}

Normal Box::GetNormal(const int face_hit) const
{
	switch (face_hit) 
	{
		case 0:	return (Normal(-1, 0, 0));	// -x face
		case 1:	return (Normal(0, -1, 0));	// -y face
		case 2:	return (Normal(0, 0, -1));	// -z face
		case 3:	return (Normal(1, 0, 0));	// +x face
		case 4:	return (Normal(0, 1, 0));	// +y face
		case 5:	return (Normal(0, 0, 1));	// +z face
		default: return Normal(-999, -999, -999);
	}
}

}