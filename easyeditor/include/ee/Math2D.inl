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
