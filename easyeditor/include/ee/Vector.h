#ifndef _EASYEDITOR_VECTOR_H_
#define _EASYEDITOR_VECTOR_H_

#include <iostream>

#include <math.h>

namespace ee
{

class Vector
{
public:
	float x, y;

public:
	Vector();
	Vector(float x, float y);
	Vector(double x, double y);
	Vector(int x, int y);
	Vector(const Vector& p);

	Vector& operator = (const Vector& p);

	void SetInvalid();
	bool IsValid() const;

	void Set(float x, float y);

	bool operator != (const Vector& v) const;
	bool operator == (const Vector& v) const;

	void operator += (const Vector& v);
	void operator -= (const Vector& v);
	void operator *= (float f);
	void operator /= (float f);

	Vector operator + (const Vector& v) const;
	Vector operator - (const Vector& v) const;
	Vector operator * (float scale) const;
	Vector operator / (float scale) const;

	Vector operator - () const;

	float Length() const;
	void Normalize();

}; // Vector

float vec_cross(const Vector& a, const Vector& b);
float vec_dot(const Vector& a, const Vector& b);

std::istream& operator >> (std::istream& is, Vector& pos);
std::ostream& operator << (std::ostream& os, const Vector& pos);

class VectorCmp
{
public:
	bool operator () (const Vector& p0, const Vector& p1) const;
}; // VectorCmp

class VectorCmpX
{
public:
	bool operator () (const Vector& p0, const Vector& p1) const;
}; // VectorCmp

class VectorCmpY
{
public:
	bool operator () (const Vector& p0, const Vector& p1) const;
}; // VectorCmp

}

#include "Vector.inl"

#endif // _EASYEDITOR_VECTOR_H_