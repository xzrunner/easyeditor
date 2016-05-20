#include "CircleShape.h"
#include "CirclePropertySetting.h"

#include <ee/Math2D.h>
#include <ee/EE_RVG.h>

namespace eshape
{

CircleShape::CircleShape(const sm::vec2& center, float radius)
	: m_core(center, radius)
{
}

CircleShape* CircleShape::Clone() const
{
	return new CircleShape(*this);
}

void CircleShape::Translate(const sm::vec2& offset)
{
	m_core.SetCenter(m_core.GetCenter() + offset);
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
	
	float radius = value["radius"].asDouble();

	m_core = s2::CircleShape(center, radius);
}

void CircleShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	const sm::vec2& center = m_core.GetCenter();
	value["x"] = center.x;
	value["y"] = center.y;

	value["radius"] = m_core.GetRadius();
}

}