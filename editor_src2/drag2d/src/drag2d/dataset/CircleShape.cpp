#include "CircleShape.h"

#include "common/Math.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

CircleShape::CircleShape()
	: center(0, 0)
	, radius(1)
	, m_rect(center, radius, radius)
{
}

CircleShape::CircleShape(const CircleShape& circle)
{
	center = circle.center;
	radius = circle.radius;
	m_rect = circle.m_rect;
}

CircleShape::CircleShape(const Vector& center, float radius)
	: center(center)
	, radius(radius)
	, m_rect(center, radius, radius)
{
}

CircleShape* CircleShape::clone() const
{
	return new CircleShape(*this);
}

bool CircleShape::isContain(const Vector& pos) const
{
	return Math::getDistance(center, pos) < QUERY_ACCURACY;
}

bool CircleShape::isIntersect(const Rect& rect) const
{
	const float cx = (rect.xMin + rect.xMax) * 0.5f,
		cy = (rect.yMin + rect.yMax) * 0.5f;

	const float dis = Math::getDistance(center, Vector(cx, cy));
	if (dis > Vector(rect.xMax - cx, rect.yMax - cy).length() + radius)
		return false;

	if (Math::isPointInCircle(Vector(rect.xMin, rect.yMin), center, radius))
		return true;
	if (Math::isPointInCircle(Vector(rect.xMax, rect.yMin), center, radius))
		return true;
	if (Math::isPointInCircle(Vector(rect.xMax, rect.yMax), center, radius))
		return true;
	if (Math::isPointInCircle(Vector(rect.xMin, rect.yMax), center, radius))
		return true;
	if (Math::isPointInRect(center, rect))
		return true;

	if (center.x > rect.xMin && center.x < rect.xMax)
	{
		if (dis < rect.yMax - cy + radius)
			return true;
	}
	if (center.y > rect.yMin && center.y < rect.yMax)
	{
		if (dis < rect.xMax - cx + radius)
			return true;
	}

	return false;
}

void CircleShape::draw(const Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	PrimitiveDraw::drawCircle(center, radius, false, 3, color, 32);
}

} // d2d