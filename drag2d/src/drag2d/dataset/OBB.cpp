#include "OBB.h"

#include "common/Math.h"

namespace d2d
{

OBB::OBB()
{
	m_angle = 0.0f;
}

OBB::OBB(const OBB& obb)
	: m_rect(obb.m_rect)
	, m_position(obb.m_position)
	, m_angle(obb.m_angle)
{
}

OBB* OBB::Clone() const
{
	return new OBB(*this);
}

void OBB::SetTransform(const Vector& position, const Vector& offset, float angle)
{
	m_position = position + (Math::rotateVector(-offset, angle) + offset);
	m_angle = angle;
}

void OBB::SetMirror(bool xmirror, bool ymirror)
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

bool OBB::isContain(const Vector& pos) const
{
	const Vector trans = Math::rotateVector(pos - m_position, -m_angle);
	return Math::isPointInRect(trans, m_rect);
}

bool OBB::isContain(const Rect& rect) const
{
	Vector trans;
	trans = Math::rotateVector(Vector(m_rect.xMin, m_rect.yMax), m_angle) + m_position;
	if (!Math::isPointInRect(trans, rect)) return false;
	trans = Math::rotateVector(Vector(m_rect.xMin, m_rect.yMin), m_angle) + m_position;
	if (!Math::isPointInRect(trans, rect)) return false;
	trans = Math::rotateVector(Vector(m_rect.xMax, m_rect.yMin), m_angle) + m_position;
	if (!Math::isPointInRect(trans, rect)) return false;
	trans = Math::rotateVector(Vector(m_rect.xMax, m_rect.yMax), m_angle) + m_position;
	if (!Math::isPointInRect(trans, rect)) return false;
	return true;
}

bool OBB::isIntersect(const Rect& rect) const
{
	// 0 3
	// 1 2
	std::vector<Vector> bound;
	Vector trans;
	trans = Math::rotateVector(Vector(m_rect.xMin, m_rect.yMax), m_angle) + m_position;
	bound.push_back(trans);
	if (Math::isPointInRect(trans, rect)) return true;
	trans = Math::rotateVector(Vector(m_rect.xMin, m_rect.yMin), m_angle) + m_position;
	bound.push_back(trans);
	if (Math::isPointInRect(trans, rect)) return true;
	trans = Math::rotateVector(Vector(m_rect.xMax, m_rect.yMin), m_angle) + m_position;
	bound.push_back(trans);
	if (Math::isPointInRect(trans, rect)) return true;
	trans = Math::rotateVector(Vector(m_rect.xMax, m_rect.yMax), m_angle) + m_position;
	bound.push_back(trans);
	if (Math::isPointInRect(trans, rect)) return true;

	if (Math::isPointInConvexHull(Vector(rect.xMin, rect.yMin), bound)) return true;
	if (Math::isPointInConvexHull(Vector(rect.xMin, rect.yMax), bound)) return true;
	if (Math::isPointInConvexHull(Vector(rect.xMax, rect.yMin), bound)) return true;
	if (Math::isPointInConvexHull(Vector(rect.xMax, rect.yMax), bound)) return true;

	if (Math::isSegmentIntersectRect(bound[0], bound[1], rect)) return true;
	if (Math::isSegmentIntersectRect(bound[1], bound[2], rect)) return true;
	if (Math::isSegmentIntersectRect(bound[2], bound[3], rect)) return true;
	if (Math::isSegmentIntersectRect(bound[3], bound[0], rect)) return true;

	return false;
}

float OBB::area() const
{
	return m_rect.xLength() * m_rect.yLength();
}

float OBB::width() const
{
	return m_rect.xLength();
}

float OBB::height() const
{
	return m_rect.yLength();
}

Vector OBB::center() const
{
	return m_position;
}

void OBB::getBoundPos(std::vector<Vector>& bound) const
{
	if (m_rect.isValid()) {
		bound.clear();
		bound.push_back(Math::rotateVector(Vector(m_rect.xMin, m_rect.yMin), m_angle) + m_position);
		bound.push_back(Math::rotateVector(Vector(m_rect.xMax, m_rect.yMin), m_angle) + m_position);
		bound.push_back(Math::rotateVector(Vector(m_rect.xMax, m_rect.yMax), m_angle) + m_position);
		bound.push_back(Math::rotateVector(Vector(m_rect.xMin, m_rect.yMax), m_angle) + m_position);
	}
}

void OBB::transToAABB()
{
	Vector leftLow(m_rect.xMin, m_rect.yMin),
		rightTop(m_rect.xMax, m_rect.yMax);
	Vector transLeftLow = Math::rotateVector(leftLow, m_angle),
		transRightTop = Math::rotateVector(rightTop, m_angle);

	m_rect.makeInfinite();
	m_rect.combine(transLeftLow);
	m_rect.combine(transRightTop);

	m_angle = 0;
}

} // d2d