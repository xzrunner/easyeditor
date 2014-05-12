#include "CircleShape.h"
#include "CirclePropertySetting.h"

namespace libshape
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

CircleShape::CircleShape(const d2d::Vector& center, float radius)
	: center(center)
	, radius(radius)
	, m_rect(center, radius, radius)
{
}

CircleShape* CircleShape::clone() const
{
	return new CircleShape(*this);
}

bool CircleShape::isContain(const d2d::Vector& pos) const
{
	return d2d::Math::getDistance(center, pos) < QUERY_ACCURACY;
}

bool CircleShape::isIntersect(const d2d::Rect& rect) const
{
	const float cx = (rect.xMin + rect.xMax) * 0.5f,
		cy = (rect.yMin + rect.yMax) * 0.5f;

	const float dis = d2d::Math::getDistance(center, d2d::Vector(cx, cy));
	if (dis > d2d::Vector(rect.xMax - cx, rect.yMax - cy).length() + radius)
		return false;

	if (d2d::Math::isPointInCircle(d2d::Vector(rect.xMin, rect.yMin), center, radius))
		return true;
	if (d2d::Math::isPointInCircle(d2d::Vector(rect.xMax, rect.yMin), center, radius))
		return true;
	if (d2d::Math::isPointInCircle(d2d::Vector(rect.xMax, rect.yMax), center, radius))
		return true;
	if (d2d::Math::isPointInCircle(d2d::Vector(rect.xMin, rect.yMax), center, radius))
		return true;
	if (d2d::Math::isPointInRect(center, rect))
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

void CircleShape::draw(const d2d::Colorf& color/* = d2d::Colorf(0, 0, 0)*/) const
{
	d2d::PrimitiveDraw::drawCircle(center, radius, false, 3, color, 32);
}

d2d::IPropertySetting* CircleShape::createPropertySetting(d2d::EditPanel* editPanel)
{
	return new CirclePropertySetting(editPanel, this);
}

}