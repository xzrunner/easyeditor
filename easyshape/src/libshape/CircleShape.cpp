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

CircleShape* CircleShape::Clone() const
{
	return new CircleShape(*this);
}

bool CircleShape::IsContain(const d2d::Vector& pos) const
{
	return d2d::Math2D::GetDistance(center, pos) < QUERY_ACCURACY;
}

bool CircleShape::IsIntersect(const d2d::Rect& rect) const
{
	const float cx = (rect.xmin + rect.xmax) * 0.5f,
		cy = (rect.ymin + rect.ymax) * 0.5f;

	const float dis = d2d::Math2D::GetDistance(center, d2d::Vector(cx, cy));
	if (dis > d2d::Vector(rect.xmax - cx, rect.ymax - cy).Length() + radius)
		return false;

	if (d2d::Math2D::IsPointInCircle(d2d::Vector(rect.xmin, rect.ymin), center, radius))
		return true;
	if (d2d::Math2D::IsPointInCircle(d2d::Vector(rect.xmax, rect.ymin), center, radius))
		return true;
	if (d2d::Math2D::IsPointInCircle(d2d::Vector(rect.xmax, rect.ymax), center, radius))
		return true;
	if (d2d::Math2D::IsPointInCircle(d2d::Vector(rect.xmin, rect.ymax), center, radius))
		return true;
	if (d2d::Math2D::IsPointInRect(center, rect))
		return true;

	if (center.x > rect.xmin && center.x < rect.xmax)
	{
		if (dis < rect.ymax - cy + radius)
			return true;
	}
	if (center.y > rect.ymin && center.y < rect.ymax)
	{
		if (dis < rect.xmax - cx + radius)
			return true;
	}

	return false;
}

void CircleShape::Translate(const d2d::Vector& offset)
{
	center += offset;
}

void CircleShape::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color) const
{
	d2d::PrimitiveDraw::DrawCircle(mt, center, radius, false, 3, color.multi, 32);
}

d2d::IPropertySetting* CircleShape::CreatePropertySetting(d2d::EditPanelImpl* stage)
{
	return new CirclePropertySetting(stage, this);
}

void CircleShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	d2d::Shape::LoadFromFile(value, dir);

	center.x = value["x"].asDouble();
	center.y = value["y"].asDouble();
	radius = value["radius"].asDouble();

	m_rect = d2d::Rect(center, radius, radius);
}

void CircleShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	d2d::Shape::StoreToFile(value, dir);

	value["x"] = center.x;
	value["y"] = center.y;
	value["radius"] = radius;
}

}