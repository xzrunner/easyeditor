#include "RectShape.h"
#include "RectPropertySetting.h"

namespace libshape
{

RectShape::RectShape()
	: m_rect(d2d::Vector(0, 0), 1, 1)
{
}

RectShape::RectShape(const RectShape& rect)
	: m_rect(rect.m_rect)
{
}

RectShape::RectShape(const d2d::Vector& p0, const d2d::Vector& p1)
	: m_rect(p0, p1)
{
}

RectShape::RectShape(const d2d::Vector& center, float hWidth, float hHeight)
	: m_rect(center, hWidth, hHeight)
{
}

RectShape* RectShape::Clone() const
{
	return new RectShape(*this);
}

bool RectShape::isContain(const d2d::Vector& pos) const
{
	return d2d::Math::isPointInRect(pos, m_rect);
}

bool RectShape::isIntersect(const d2d::Rect& rect) const
{
	return d2d::Math::isRectIntersectRect(rect, m_rect);
}

void RectShape::Translate(const d2d::Vector& offset)
{
	m_rect.translate(offset);
}

void RectShape::draw(const d2d::Matrix& mt, 
					 const d2d::Colorf& color/* = d2d::Colorf(0, 0, 0)*/) const
{
	d2d::PrimitiveDraw::rect(mt, m_rect, m_style);
}

d2d::IPropertySetting* RectShape::createPropertySetting(d2d::EditPanelImpl* stage)
{
	return new RectPropertySetting(stage, this);
}

void RectShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	d2d::IShape::LoadFromFile(value, dir);

	m_rect.xMin = value["xmin"].asDouble();
	m_rect.xMax = value["xmax"].asDouble();
	m_rect.yMin = value["ymin"].asDouble();
	m_rect.yMax = value["ymax"].asDouble();
}

void RectShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	d2d::IShape::StoreToFile(value, dir);

	value["xmin"] = m_rect.xMin;
	value["xmax"] = m_rect.xMax;
	value["ymin"] = m_rect.yMin;
	value["ymax"] = m_rect.yMax;
}

}