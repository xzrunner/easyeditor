#include "AABB.h"

#include "common/Math.h"

namespace d2d
{

AABB::AABB()
{
}

AABB::AABB(const AABB& aabb)
{
	m_rect = aabb.m_rect;	
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

bool AABB::isContain(const Vector& pos) const
{
	return Math::isPointInRect(pos, m_rect);
}

bool AABB::isContain(const Rect& rect) const
{
	return Math::isRectContainRect(m_rect, rect);
}

bool AABB::isIntersect(const Rect& rect) const
{
	return Math::isRectIntersectRect(m_rect, rect);
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
	return Vector(m_rect.xCenter(), m_rect.yCenter());
}

void AABB::getBoundPos(std::vector<Vector>& bound) const
{
	bound.clear();
	bound.push_back(Vector(m_rect.xMin, m_rect.yMin));
	bound.push_back(Vector(m_rect.xMax, m_rect.yMin));
	bound.push_back(Vector(m_rect.xMax, m_rect.yMax));
	bound.push_back(Vector(m_rect.xMin, m_rect.yMax));
}

} // d2d