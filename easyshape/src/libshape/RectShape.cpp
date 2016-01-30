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

bool RectShape::IsContain(const d2d::Vector& pos) const
{
	return d2d::Math2D::IsPointInRect(pos, m_rect);
}

bool RectShape::IsIntersect(const d2d::Rect& rect) const
{
	return d2d::Math2D::IsRectIntersectRect(rect, m_rect);
}

void RectShape::Translate(const d2d::Vector& offset)
{
	m_rect.Translate(offset);
}

void RectShape::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color) const
{
	d2d::PrimitiveDraw::DrawRect(mt, m_rect, m_style);
}

d2d::IPropertySetting* RectShape::CreatePropertySetting(d2d::EditPanelImpl* stage)
{
	return new RectPropertySetting(stage, this);
}

void RectShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	d2d::Shape::LoadFromFile(value, dir);

	m_rect.xmin = value["xmin"].asDouble();
	m_rect.xmax = value["xmax"].asDouble();
	m_rect.ymin = value["ymin"].asDouble();
	m_rect.ymax = value["ymax"].asDouble();
}

void RectShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	d2d::Shape::StoreToFile(value, dir);

	value["xmin"] = m_rect.xmin;
	value["xmax"] = m_rect.xmax;
	value["ymin"] = m_rect.ymin;
	value["ymax"] = m_rect.ymax;
}

}