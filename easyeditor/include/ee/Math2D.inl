#ifndef _EASYEDITOR_MATH2D_INL_
#define _EASYEDITOR_MATH2D_INL_

#include <sm_const.h>

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
bool Math2D::IsTheSamePos(const sm::vec2& p0, const sm::vec2& p1, const float tolerance)
{
	return fabs(p0.x - p1.x) <= tolerance
		&& fabs(p0.y - p1.y) <= tolerance;
}

inline
bool Math2D::IsPointAtSegmentLeft(const sm::vec2& p, const sm::vec2& s, const sm::vec2& e)
{
	return (p.y - s.y) * (e.x - s.x) - (p.x - s.x) * (e.y - s.y) > FLT_EPSILON;
}

inline
bool Math2D::IsRectIntersectRect(const sm::rect& r0, const sm::rect& r1)
{
	return !(r0.xmin >= r1.xmax || r0.xmax <= r1.xmin || r0.ymin >= r1.ymax || r0.ymax <= r1.ymin);
}

inline
bool Math2D::IsRectContainRect(const sm::rect& r0, const sm::rect& r1)
{
	return r1.xmin >= r0.xmin && r1.xmax <= r0.xmax 
		&& r1.ymin >= r0.ymin && r1.ymax <= r0.ymax;
}

inline
bool Math2D::IsSegmentIntersectRect(const sm::vec2& b, const sm::vec2& e, const sm::rect& aabb)
{
	return IsSegmentIntersectRect(b.x, b.y, e.x, e.y, aabb.xmin, aabb.ymin, aabb.xmax, aabb.ymax);
}

inline
bool Math2D::IsPointInRect(const sm::vec2& pos, const sm::rect& aabb)
{
	return pos.x > aabb.xmin && pos.x < aabb.xmax
		&& pos.y > aabb.ymin && pos.y < aabb.ymax;
}

inline
bool Math2D::IsPointInRect(const sm::vec2& pos, const sm::vec2& center, float hw, float hh)
{
	return pos.x > center.x - hw && pos.x < center.x + hw 
		&& pos.y > center.y - hh && pos.y < center.y + hh;
}

inline
float Math2D::FindXOnSeg(float x1, float y1, float x2, float y2, float y)
{
	if (y1 == y2)  {
		return (std::min)(x1, x2);
	} else {
		return (y - y2) * (x1 - x2) / (y1 - y2) + x2;
	}
}

inline
float Math2D::FindYOnSeg(float x1, float y1, float x2, float y2, float x)
{
	if (x1 == x2)  {
		return (std::min)(y1, y2);
	} else {
		return (x - x2) * (y1 - y2) / (x1 - x2) + y2;
	}
}

inline
float Math2D::FindXOnSeg(const sm::vec2& b, const sm::vec2& e, float y)
{
	return FindXOnSeg(b.x, b.y, e.x, e.y, y);
}

inline
float Math2D::FindYOnSeg(const sm::vec2& b, const sm::vec2& e, float x)
{
	return FindYOnSeg(b.x, b.y, e.x, e.y, x);
}

inline
float Math2D::GetDistance(const sm::vec2& p0, const sm::vec2& p1)
{
	const float dx = p0.x - p1.x,
		dy = p0.y - p1.y;
	return sqrt(dx * dx + dy * dy);
}

inline
float Math2D::GetDistanceSquare(const sm::vec2& p0, const sm::vec2& p1)
{
	const float dx = p0.x - p1.x,
		dy = p0.y - p1.y;
	return dx * dx + dy * dy;
}

inline
sm::vec2 Math2D::GetTriGravityCenter(const sm::vec2& p0, const sm::vec2& p1, const sm::vec2& p2)
{
	return sm::vec2((p0.x + p1.x + p2.x) / 3, (p0.y + p1.y + p2.y) / 3);
}

inline
int Math2D::GetNextIdxInRing(int sz, int curr, int step) 
{
	return (curr + sz + step) % sz;
}

inline
bool Math2D::IsTwoPointsSame(const sm::vec2& p0, const sm::vec2& p1) 
{
	return fabs(p0.x - p1.x) < SM_RAD_TO_DEG
		&& fabs(p0.y - p1.y) < SM_RAD_TO_DEG;
}

}

#endif // _EASYEDITOR_MATH2D_INL_
