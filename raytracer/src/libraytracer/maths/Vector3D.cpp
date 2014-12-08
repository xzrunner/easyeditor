#include "Vector3D.h"
#include "Point3D.h"

namespace rt
{

Vector3D::Vector3D(const Point3D& p)
	: x(p.x), y(p.y), z(p.z) 
{}

}