#include "OBB.h"
#include "Math2D.h"

namespace ee
{

void OBB::SetTransform(const Vector& position, const Vector& offset, float angle)
{
	m_position = position + (Math2D::RotateVector(-offset, angle) + offset);
	m_angle = angle;
}

bool OBB::IsContain(const Vector& pos) const
{
	const Vector trans = Math2D::RotateVector(pos - m_position, -m_angle);
	return Math2D::IsPointInRect(trans, m_rect);
}

bool OBB::IsContain(const Rect& rect) const
{
	Vector trans;
	trans = Math2D::RotateVector(Vector(m_rect.xmin, m_rect.ymax), m_angle) + m_position;
	if (!Math2D::IsPointInRect(trans, rect)) return false;
	trans = Math2D::RotateVector(Vector(m_rect.xmin, m_rect.ymin), m_angle) + m_position;
	if (!Math2D::IsPointInRect(trans, rect)) return false;
	trans = Math2D::RotateVector(Vector(m_rect.xmax, m_rect.ymin), m_angle) + m_position;
	if (!Math2D::IsPointInRect(trans, rect)) return false;
	trans = Math2D::RotateVector(Vector(m_rect.xmax, m_rect.ymax), m_angle) + m_position;
	if (!Math2D::IsPointInRect(trans, rect)) return false;
	return true;
}

bool OBB::IsIntersect(const Rect& rect) const
{
	// 0 3
	// 1 2
	std::vector<Vector> bound;
	Vector trans;
	trans = Math2D::RotateVector(Vector(m_rect.xmin, m_rect.ymax), m_angle) + m_position;
	bound.push_back(trans);
	if (Math2D::IsPointInRect(trans, rect)) return true;
	trans = Math2D::RotateVector(Vector(m_rect.xmin, m_rect.ymin), m_angle) + m_position;
	bound.push_back(trans);
	if (Math2D::IsPointInRect(trans, rect)) return true;
	trans = Math2D::RotateVector(Vector(m_rect.xmax, m_rect.ymin), m_angle) + m_position;
	bound.push_back(trans);
	if (Math2D::IsPointInRect(trans, rect)) return true;
	trans = Math2D::RotateVector(Vector(m_rect.xmax, m_rect.ymax), m_angle) + m_position;
	bound.push_back(trans);
	if (Math2D::IsPointInRect(trans, rect)) return true;

	if (Math2D::IsPointInConvexHull(Vector(rect.xmin, rect.ymin), bound)) return true;
	if (Math2D::IsPointInConvexHull(Vector(rect.xmin, rect.ymax), bound)) return true;
	if (Math2D::IsPointInConvexHull(Vector(rect.xmax, rect.ymin), bound)) return true;
	if (Math2D::IsPointInConvexHull(Vector(rect.xmax, rect.ymax), bound)) return true;

	if (Math2D::IsSegmentIntersectRect(bound[0], bound[1], rect)) return true;
	if (Math2D::IsSegmentIntersectRect(bound[1], bound[2], rect)) return true;
	if (Math2D::IsSegmentIntersectRect(bound[2], bound[3], rect)) return true;
	if (Math2D::IsSegmentIntersectRect(bound[3], bound[0], rect)) return true;

	return false;
}

void OBB::GetBoundPos(std::vector<Vector>& bound) const
{
	if (m_rect.IsValid()) {
		bound.clear();
		bound.push_back(Math2D::RotateVector(Vector(m_rect.xmin, m_rect.ymin), m_angle) + m_position);
		bound.push_back(Math2D::RotateVector(Vector(m_rect.xmax, m_rect.ymin), m_angle) + m_position);
		bound.push_back(Math2D::RotateVector(Vector(m_rect.xmax, m_rect.ymax), m_angle) + m_position);
		bound.push_back(Math2D::RotateVector(Vector(m_rect.xmin, m_rect.ymax), m_angle) + m_position);
	}
}

void OBB::TransToAABB()
{
	Vector leftLow(m_rect.xmin, m_rect.ymin),
		rightTop(m_rect.xmax, m_rect.ymax);
	Vector transLeftLow = Math2D::RotateVector(leftLow, m_angle),
		transRightTop = Math2D::RotateVector(rightTop, m_angle);

	m_rect.MakeInfinite();
	m_rect.Combine(transLeftLow);
	m_rect.Combine(transRightTop);

	m_angle = 0;
}

}