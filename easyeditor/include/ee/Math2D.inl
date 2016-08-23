#ifndef _EASYEDITOR_MATH2D_INL_
#define _EASYEDITOR_MATH2D_INL_

#include <sm_const.h>

namespace ee
{

inline
bool Math2D::IsTheSamePos(const sm::vec2& p0, const sm::vec2& p1, const float tolerance)
{
	return fabs(p0.x - p1.x) <= tolerance
		&& fabs(p0.y - p1.y) <= tolerance;
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
