#include "CircleShape.h"
#include "CirclePropertySetting.h"

#include <ee/Math2D.h>

#include <sprite2/S2_RVG.h>

namespace eshape
{

CircleShape::CircleShape(const CircleShape& circle)
	: ee::Shape(circle)
	, s2::CircleShape(circle)
{
}

CircleShape::CircleShape(const sm::vec2& center, float radius)
	: s2::CircleShape(center, radius)
{
}

void CircleShape::Translate(const sm::vec2& offset)
{
	SetCenter(m_center + offset);
}

ee::PropertySetting* CircleShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new CirclePropertySetting(stage, this);
}

void CircleShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	sm::vec2 center;
	center.x = value["x"].asDouble();
	center.y = value["y"].asDouble();
	SetCenter(center);
	
	float radius = value["radius"].asDouble();
	SetRadius(radius);
}

void CircleShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	value["x"] = m_center.x;
	value["y"] = m_center.y;

	value["radius"] = m_radius;
}

}