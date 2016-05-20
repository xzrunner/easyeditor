#include "PointShape.h"

namespace eshape
{

void PointShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	sm::vec2 pos;
	pos.x = static_cast<float>(value["x"].asDouble());
	pos.y = static_cast<float>(value["y"].asDouble());
	m_core.SetPos(pos);
}

void PointShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	const sm::vec2& pos = m_core.GetPos();
	value["x"] = pos.x;
	value["y"] = pos.y;
}

}