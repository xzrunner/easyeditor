#include "AABB.h"
#include "Math2D.h"

namespace ee
{

void AABB::SetTransform(const Vector& position, const Vector& offset, float angle)
{
	m_position = position + (Math2D::RotateVector(-offset, angle) + offset);
}

bool AABB::IsContain(const Vector& pos) const
{
	return Math2D::IsPointInRect(pos - m_position, m_rect);
}

bool AABB::IsContain(const Rect& rect) const
{
	Rect r(rect);
	r.Translate(-m_position);
	return Math2D::IsRectContainRect(m_rect, r);
}

bool AABB::IsIntersect(const Rect& rect) const
{
	Rect r(rect);
	r.Translate(-m_position);
	return Math2D::IsRectIntersectRect(m_rect, r);
}

}