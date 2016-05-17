#include "AABB.h"
#include "Math2D.h"

namespace ee
{

void AABB::SetTransform(const sm::vec2& position, const sm::vec2& offset, float angle)
{
	m_position = position + (Math2D::RotateVector(-offset, angle) + offset);
}

bool AABB::IsContain(const sm::vec2& pos) const
{
	return Math2D::IsPointInRect(pos - m_position, m_rect);
}

bool AABB::IsContain(const sm::rect& rect) const
{
	sm::rect r(rect);
	r.Translate(-m_position);
	return Math2D::IsRectContainRect(m_rect, r);
}

bool AABB::IsIntersect(const sm::rect& rect) const
{
	sm::rect r(rect);
	r.Translate(-m_position);
	return Math2D::IsRectIntersectRect(m_rect, r);
}

}