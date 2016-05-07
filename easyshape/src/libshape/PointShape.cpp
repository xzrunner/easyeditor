#include "PointShape.h"

#include <ee/Math2D.h>
#include <ee/EE_RVG.h>

namespace eshape
{

static const float RADIUS = 5;

PointShape::PointShape()
{
}

PointShape::PointShape(const ee::Vector& pos)
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

bool PointShape::IsContain(const ee::Vector& pos) const
{
	return ee::Math2D::IsPointInRect(pos, m_rect);
}

bool PointShape::IsIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsRectIntersectRect(rect, m_rect);
}

void PointShape::Translate(const ee::Vector& offset)
{
	m_pos += offset;
	m_rect.Translate(offset);
}

void PointShape::Draw(const sm::mat4& mt, const ee::RenderColor& color) const
{
	ee::Vector c = ee::Math2D::TransVector(m_pos, mt);
	float r = ee::Math2D::TransLen(RADIUS, mt);
	ee::RVG::Color(color.multi);
	ee::RVG::Circle(c, r, true);
}

ee::PropertySetting* PointShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return NULL;
}

void PointShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	m_pos.x = static_cast<float>(value["x"].asDouble());
	m_pos.y = static_cast<float>(value["y"].asDouble());
}

void PointShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	value["x"] = m_pos.x;
	value["y"] = m_pos.y;
}

}