#ifndef _EASYEDITOR_MATH2D_INL_
#define _EASYEDITOR_MATH2D_INL_

#include "Vector.h"
#include "Rect.h"

namespace ee
{

inline
bool Math2D::IsBetween(float bound0, float bound1, float test)
{
	if (bound0 < bound1) {
		return test < bound1 + FLT_EPSILON && test > bound0 - FLT_EPSILON;
	} else {
		return test < bound0 + FLT_EPSILON && test > bound1 - FLT_EPSILON;
	}
}

inline
bool Math2D::IsTheSamePos(const Vector& p0, const Vector& p1, const float tolerance)
{
	return fabs(p0.x - p1.x) <= tolerance
		&& fabs(p0.y - p1.y) <= tolerance;
}

inline
bool Math2D::IsPointAtSegmentLeft(const Vector& p, const Vector& s, const Vector& e)
{
	return (p.y - s.y) * (e.x - s.x) - (p.x - s.x) * (e.y - s.y) > FLT_EPSILON;
}

inline
bool Math2D::IsRectIntersectRect(const Rect& r0, const Rect& r1)
{
	return !(r0.xmin >= r1.xmax || r0.xmax <= r1.xmin || r0.ymin >= r1.ymax || r0.ymax <= r1.ymin);
}

inline
bool Math2D::IsRectContainRect(const Rect& r0, const Rect& r1)
{
	return r1.xmin >= r0.xmin && r1.xmax <= r0.xmax 
		&& r1.ymin >= r0.ymin && r1.ymax <= r0.ymax;
}

inline
bool Math2D::IsSegmentIntersectRect(const Vector& b, const Vector& e, const Rect& aabb)
{
	return IsSegmentIntersectRect(b.x, b.y, e.x, e.y, aabb.xmin, aabb.ymin, aabb.xmax, aabb.ymax);
}

inline
bool Math2D::IsPointInRect(const Vector& pos, const Rect& aabb)
{
	return pos.x > aabb.xmin && pos.x < aabb.xmax
		&& pos.y > aabb.ymin && pos.y < aabb.ymax;
}

inline
bool Math2D::IsPointInRect(const Vector& pos, const Vector& center, float hw, float hh)
{
	return pos.x > center.x - hw && pos.x < center.x + hw 
		&& pos.y > center.y - hh && pos.y < center.y + hh;
}

inline
float Math2D::FindXOnSeg(float x1, float y1, float x2, float y2, float y)
{
	if (y1 == y2)  {
		return std::min(x1, x2);
	} else {
		return (y - y2) * (x1 - x2) / (y1 - y2) + x2;
	}
}

inline
float Math2D::FindYOnSeg(float x1, float y1, float x2, float y2, float x)
{
	if (x1 == x2)  {
		return std::min(y1, y2);
	} else {
		return (x - x2) * (y1 - y2) / (x1 - x2) + y2;
	}
}

inline
float Math2D::FindXOnSeg(const Vector& b, const Vector& e, float y)
{
	return FindXOnSeg(b.x, b.y, e.x, e.y, y);
}

inline
float Math2D::FindYOnSeg(const Vector& b, const Vector& e, float x)
{
	return FindYOnSeg(b.x, b.y, e.x, e.y, x);
}

inline
float Math2D::GetDistance(const Vector& p0, const Vector& p1)
{
	const float dx = p0.x - p1.x,
		dy = p0.y - p1.y;
	return sqrt(dx * dx + dy * dy);
}

inline
float Math2D::GetDistanceSquare(const Vector& p0, const Vector& p1)
{
	const float dx = p0.x - p1.x,
		dy = p0.y - p1.y;
	return dx * dx + dy * dy;
}

inline
Vector Math2D::GetTriGravityCenter(const Vector& p0, const Vector& p1, const Vector& p2)
{
	return Vector((p0.x + p1.x + p2.x) / 3, (p0.y + p1.y + p2.y) / 3);
}

inline
int Math2D::GetNextIdxInRing(int sz, int curr, int step) 
{
	return (curr + sz + step) % sz;
}

inline
bool Math2D::IsTwoPointsSame(const Vector& p0, const Vector& p1) 
{
	return fabs(p0.x - p1.x) < LARGE_EPSILON
		&& fabs(p0.y - p1.y) < LARGE_EPSILON;
}

}

#endif // _EASYEDITOR_MATH2D_INL_
