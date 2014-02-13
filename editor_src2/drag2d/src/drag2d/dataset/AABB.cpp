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

AABB* AABB::clone() const
{
	return new AABB(*this);
}

void AABB::loadFromTextFile(std::ifstream& fin)
{
	fin >> m_rect.xMin >> m_rect.xMax >> m_rect.yMin >> m_rect.yMax;
}

void AABB::storeToTextFile(std::ofstream& fout) const
{
	fout << m_rect.xMin << " " << m_rect.xMax << " "
		<< m_rect.yMin << " " << m_rect.yMax << '\n';
}

void AABB::setTransform(const Vector& position, const Vector& offset, float angle)
{
// 	Vector center_offset = Math::rotateVector(-offset, angle) + offset;
// 	Vector center = position + center_offset;
// 	m_rect.translate(Vector(center.x - m_rect.xCenter(), center.y - m_rect.yCenter()));

	m_position = position;
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
//	return Vector(m_rect.xCenter(), m_rect.yCenter());
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