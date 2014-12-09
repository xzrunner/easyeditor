#include "Vector3D.h"
#include "Point3D.h"
#include "Normal.h"

namespace rt
{

Vector3D::Vector3D(const Point3D& p)
	: x(p.x), y(p.y), z(p.z) 
{}

Vector3D::Vector3D(const Normal& n)
	: x(n.x), y(n.y), z(n.z) 
{}


}