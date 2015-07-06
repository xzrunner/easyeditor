#include "AABB.h"

#include "common/Math.h"

namespace d2d
{

AABB::AABB()
{
}

AABB::AABB(const AABB& aabb)
	: m_rect(aabb.m_rect)
	, m_position(aabb.m_position)
{
}

AABB* AABB::Clone() const
{
	return new AABB(*this);
}

void AABB::SetTransform(const Vector& position, const Vector& offset, float angle)
{
	m_position = position + (Math::rotateVector(-offset, angle) + offset);
}

void AABB::SetMirror(bool xmirror, bool ymirror)
{
	if (xmirror) {
		m_rect.xMin = -m_rect.xMin;
		m_rect.xMax = -m_rect.xMax;
		std::swap(m_rect.xMin, m_rect.xMax);
	}
	if (ymirror) {
		m_rect.yMin = -m_rect.yMin;
		m_rect.yMax = -m_rect.yMax;
		std::swap(m_rect.yMin, m_rect.yMax);
	}
}

bool AABB::isContain(const Vector& pos) const
{
	return Math::isPointInRect(pos - m_position, m_rect);
}

bool AABB::isContain(const Rect& rect) const
{
	Rect r(rect);
	r.translate(-m_position);
	return Math::isRectContainRect(m_rect, r);
}

bool AABB::isIntersect(const Rect& rect) const
{
	Rect r(rect);
	r.translate(-m_position);
	return Math::isRectIntersectRect(m_rect, r);
}

float AABB::area() const
{
	return m_rect.xLength() * m_rect.yLength();
}

float AABB::width() const
{
	return m_rect.xLength();
}

float AABB::height() const
{
	return m_rect.yLength();
}

Vector AABB::center() const
{
	return m_position;
}

void AABB::getBoundPos(std::vector<Vector>& bound) const
{
	bound.clear();
	bound.push_back(Vector(m_rect.xMin, m_rect.yMin) + m_position);
	bound.push_back(Vector(m_rect.xMax, m_rect.yMin) + m_position);
	bound.push_back(Vector(m_rect.xMax, m_rect.yMax) + m_position);
	bound.push_back(Vector(m_rect.xMin, m_rect.yMax) + m_position);
}

} // d2d