#ifndef _RT_VECTOR3D_H_
#define _RT_VECTOR3D_H_

#include <math.h>

namespace rt
{

class Point3D;
class Normal;

class Vector3D
{
public:
	double x, y, z;

public:
	Vector3D() : x(0), y(0), z(0) {}
	Vector3D(double x, double y, double z)
		: x(x), y(y), z(z) {}
	Vector3D(const Point3D& p);
	Vector3D(const Normal& n);

	// unary minus
	Vector3D operator - () const;

	// multiplication by a double on the right
	Vector3D operator * (const double a) const;

	// division by a double
	Vector3D operator / (const double a) const;

	// addition
	Vector3D operator + (const Vector3D& v) const;

	// subtraction
	Vector3D operator - (const Vector3D& v) const;

	// dot product
	double operator * (const Vector3D& v) const;

	// cross product
	Vector3D operator ^ (const Vector3D& v) const;

	void Normalize();

	double Length() const;
	double LenSquared() const;

}; // Vector3D

inline
Vector3D Vector3D::operator - () const {
	return Vector3D(-x, -y, -z);
}

inline Vector3D 
Vector3D::operator * (const double a) const {
	return Vector3D(x * a, y * a, z * a);
}

inline Vector3D 
Vector3D::operator / (const double a) const {
	return Vector3D(x / a, y / a, z / a);
}

inline Vector3D 
Vector3D::operator + (const Vector3D& v) const {
	return Vector3D(x + v.x, y + v.y, z + v.z);
}

inline Vector3D 
Vector3D::operator - (const Vector3D& v) const {
	return Vector3D(x - v.x, y - v.y, z - v.z);
}

inline double 
Vector3D::operator * (const Vector3D& v) const {
	return (x * v.x + y * v.y + z * v.z);
}

inline Vector3D 
Vector3D::operator ^ (const Vector3D& v) const {
	return (Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x));
}

inline void Vector3D::
Normalize()
{
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}

inline double Vector3D::
LenSquared() const
{
	return x * x + y * y + z * z;
}

inline double Vector3D::
Length() const
{
	return sqrt(LenSquared());
}

inline Vector3D
operator * (const double a, const Vector3D& v) {
	return Vector3D(a * v.x, a * v.y, a * v.z);
}

}

#endif // _RT_VECTOR3D_H_