#include "RectShape.h"
#include "RectPropertySetting.h"

#include <ee/Math2D.h>
#include <ee/EE_RVG.h>

namespace eshape
{

RectShape::RectShape()
	: m_rect(ee::Vector(0, 0), 1, 1)
{
}

RectShape::RectShape(const RectShape& rect)
	: m_rect(rect.m_rect)
{
}

RectShape::RectShape(const ee::Vector& p0, const ee::Vector& p1)
	: m_rect(p0, p1)
{
}

RectShape::RectShape(const ee::Vector& center, float hWidth, float hHeight)
	: m_rect(center, hWidth, hHeight)
{
}

RectShape* RectShape::Clone() const
{
	return new RectShape(*this);
}

bool RectShape::IsContain(const ee::Vector& pos) const
{
	return ee::Math2D::IsPointInRect(pos, m_rect);
}

bool RectShape::IsIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsRectIntersectRect(rect, m_rect);
}

void RectShape::Translate(const ee::Vector& offset)
{
	m_rect.Translate(offset);
}

void RectShape::Draw(const ee::Matrix& mt, const ee::ColorTrans& color) const
{
	ee::Vector min(m_rect.xmin, m_rect.ymin),
		max(m_rect.xmax, m_rect.ymax);
	min = ee::Math2D::TransVector(min, mt);
	max = ee::Math2D::TransVector(max, mt);
	ee::RVG::Rect(min, max, false);
}

ee::PropertySetting* RectShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new RectPropertySetting(stage, this);
}

void RectShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	m_rect.xmin = value["xmin"].asDouble();
	m_rect.xmax = value["xmax"].asDouble();
	m_rect.ymin = value["ymin"].asDouble();
	m_rect.ymax = value["ymax"].asDouble();
}

void RectShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	value["xmin"] = m_rect.xmin;
	value["xmax"] = m_rect.xmax;
	value["ymin"] = m_rect.ymin;
	value["ymax"] = m_rect.ymax;
}

}