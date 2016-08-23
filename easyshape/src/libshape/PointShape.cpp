#include "PointShape.h"

namespace eshape
{

PointShape::PointShape(const PointShape& point)
	: ee::Shape(point)
	, s2::PointShape(point)
{
}

PointShape::PointShape(const sm::vec2& pos) 
	: s2::Shape()
	, ee::Shape()
	, s2::PointShape(pos) 
{
}

void PointShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	sm::vec2 v;
	v.x = static_cast<float>(value["x"].asDouble());
	v.y = static_cast<float>(value["y"].asDouble());
	SetPos(v);
}

void PointShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	value["x"] = m_pos.x;
	value["y"] = m_pos.y;
}

}