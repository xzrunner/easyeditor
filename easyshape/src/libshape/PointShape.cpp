#include "PointShape.h"

namespace libshape
{

PointShape::PointShape()
{
}

PointShape::PointShape(const d2d::Vector& pos)
	: m_pos(pos)
	, m_rect(m_pos, RADIUS, RADIUS)
{
}

PointShape::PointShape(const PointShape& point)
	: m_pos(point.m_pos)
	, m_rect(point.m_rect)
{	
}

PointShape* PointShape::Clone() const
{
	return new PointShape(*this);
}

bool PointShape::isContain(const d2d::Vector& pos) const
{
	return d2d::Math::isPointInRect(pos, m_rect);
}

bool PointShape::isIntersect(const d2d::Rect& rect) const
{
	return d2d::Math::isRectIntersectRect(rect, m_rect);
}

void PointShape::Translate(const d2d::Vector& offset)
{
	m_pos += offset;
	m_rect.translate(offset);
}

void PointShape::draw(const d2d::Matrix& mt, const d2d::Colorf& color) const
{
	d2d::PrimitiveDraw::drawCircle(mt, m_pos, RADIUS, true, 2, color);
}

d2d::IPropertySetting* PointShape::createPropertySetting(d2d::EditPanelImpl* stage)
{
	return NULL;
}

void PointShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	d2d::IShape::LoadFromFile(value, dir);

	m_pos.x = value["x"].asDouble();
	m_pos.y = value["y"].asDouble();
}

void PointShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	d2d::IShape::StoreToFile(value, dir);

	value["x"] = m_pos.x;
	value["y"] = m_pos.y;
}

}