#pragma once

#include <float.h>
#include <math.h>
#include <iostream>

static const float FLT_INVALID = FLT_MAX;

namespace d2d
{
	class Vector
	{
	public:
		Vector() : x(0), y(0) {}
		Vector(float x, float y) : x(x), y(y) {}
		Vector(int x, int y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {}
		Vector(const Vector& p) : x(p.x), y(p.y) {}

		Vector& operator = (const Vector& p) {
			x = p.x;
			y = p.y;
			return *this;
		}

		void setInvalid() { x = y = FLT_INVALID; }
		bool isValid() const { return x != FLT_INVALID && y != FLT_INVALID; }

		void set(float _x, float _y) { x = _x; y = _y; }

		bool operator != (const Vector& v) const
		{
			return x != v.x || y != v.y;
		}
		bool operator == (const Vector& v) const
		{
			return x == v.x && y == v.y;
		}

		void operator += (const Vector& v)
		{
			x += v.x; y += v.y;
		}

		void operator -= (const Vector& v)
		{
			x -= v.x; y -= v.y;
		}

		void operator *= (float f)
		{
			x *= f; y *= f;
		}

		void operator /= (float f)
		{
			x /= f; y /= f;
		}

		Vector operator + (const Vector& v) const
		{
			return Vector(x + v.x, y + v.y);
		}

		Vector operator - (const Vector& v) const
		{
			return Vector(x - v.x, y - v.y);
		}

		Vector operator * (float scale) const
		{
			return Vector(x * scale, y * scale);
		}

		Vector operator / (float scale) const
		{
			return Vector(x / scale, y / scale);
		}

		Vector operator - () const
		{
			return Vector(-x, -y);
		}

		float length() const
		{
			return sqrt(x * x + y * y);
		}

		void normalize() 
		{
			const float len = length();
			x /= len;
			y /= len;
		}

		void turnLeft90Deg() 
		{
			const float tmp = x;
			x = -y;
			y = tmp;
		}

		void toInteger() 
		{
			x = floor(x + 0.5f);
			y = floor(y + 0.5f);
		}

		float x, y;

	}; // Vector

	//inline Vector operator + (const Vector& a, const Vector& b)
	//{
	//	return Vector(a.x + b.x, a.y + b.y);
	//}

	//inline Vector operator - (const Vector& a, const Vector& b)
	//{
	//	return Vector(a.x - b.x, a.y - b.y);
	//}

	//	inline Vector operator * (const Vector& v, float s)
	//	{
	//		return Vector(v.x * s, v.y * s);
	//	}

	inline float f2Cross(const Vector& a, const Vector& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	inline float f2Dot(const Vector& a, const Vector& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	inline std::istream& operator >> (std::istream& is, Vector& pos)
	{
		is >> pos.x >> pos.y;
		return is;
	}
	inline std::ostream& operator << (std::ostream& os, const Vector& pos)
	{
		os << pos.x << " " << pos.y;
		return os;
	}

	class VectorCmp
	{
	public:
		bool operator () (const Vector& p0, const Vector& p1) const {
			return p0.x < p1.x || p0.x == p1.x && p0.y < p1.y;
		}
	}; // VectorCmp

}

