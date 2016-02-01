#ifndef _EASYEDITOR_VECTOR_INL_
#define _EASYEDITOR_VECTOR_INL_

namespace ee
{

inline 
Vector::Vector() 
	: x(0), y(0) 
{}

inline
Vector::Vector(float x, float y) 
	: x(x), y(y) 
{}

inline
Vector::Vector(double x, double y) 
	: x(static_cast<float>(x)), y(static_cast<float>(y)) 
{}

inline
Vector::Vector(int x, int y) 
	: x(static_cast<float>(x)), y(static_cast<float>(y)) 
{}

inline
Vector::Vector(const Vector& p) 
	: x(p.x), y(p.y) 
{}

inline
Vector& Vector::operator = (const Vector& p) 
{
	x = p.x;
	y = p.y;
	return *this;
}

inline
void Vector::SetInvalid() 
{ 
	x = y = FLT_INVALID; 
}

inline
bool Vector::IsValid() const 
{ 
	return x != FLT_INVALID && y != FLT_INVALID; 
}

inline
void Vector::Set(float _x, float _y) 
{ 
	x = _x; y = _y; 
}

inline
bool Vector::operator != (const Vector& v) const
{
	return x != v.x || y != v.y;
}

inline
bool Vector::operator == (const Vector& v) const
{
	return x == v.x && y == v.y;
}

inline
void Vector::operator += (const Vector& v)
{
	x += v.x; y += v.y;
}

inline
void Vector::operator -= (const Vector& v)
{
	x -= v.x; y -= v.y;
}

inline
void Vector::operator *= (float f)
{
	x *= f; y *= f;
}

inline
void Vector::operator /= (float f)
{
	x /= f; y /= f;
}

inline
Vector Vector::operator + (const Vector& v) const
{
	return Vector(x + v.x, y + v.y);
}

inline
Vector Vector::operator - (const Vector& v) const
{
	return Vector(x - v.x, y - v.y);
}

inline
Vector Vector::operator * (float scale) const
{
	return Vector(x * scale, y * scale);
}

inline
Vector Vector::operator / (float scale) const
{
	return Vector(x / scale, y / scale);
}

inline
Vector Vector::operator - () const
{
	return Vector(-x, -y);
}

inline
float Vector::Length() const
{
	return sqrt(x * x + y * y);
}

inline
void Vector::Normalize() 
{
	const float len = Length();
	if (len != 0) {
		x /= len;
		y /= len;
	}
}

inline 
float vec_cross(const Vector& a, const Vector& b)
{
	return a.x * b.y - a.y * b.x;
}

inline 
float vec_dot(const Vector& a, const Vector& b)
{
	return a.x * b.x + a.y * b.y;
}

inline 
std::istream& operator >> (std::istream& is, Vector& pos)
{
	is >> pos.x >> pos.y;
	return is;
}

inline 
std::ostream& operator << (std::ostream& os, const Vector& pos)
{
	os << pos.x << " " << pos.y;
	return os;
}

inline 
bool VectorCmp::operator () (const Vector& p0, const Vector& p1) const 
{
	return p0.x < p1.x 
		|| p0.x == p1.x && p0.y < p1.y;
}

inline
bool VectorCmpX::operator () (const Vector& p0, const Vector& p1) const 
{
	return p0.x < p1.x 
		|| p0.x == p1.x && p0.y < p1.y;
}

inline
bool VectorCmpY::operator () (const Vector& p0, const Vector& p1) const 
{
	return p0.y < p1.y
		|| p0.y == p1.y && p0.x < p1.x;
}

}

#endif // _EASYEDITOR_VECTOR_INL_