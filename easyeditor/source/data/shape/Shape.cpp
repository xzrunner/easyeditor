#include "Shape.h"
#include "StringHelper.h"

namespace ee
{

Shape::Shape() 
{ 
	static int index = 0;
	m_name = "shape" + StringHelper::ToString(index++);
}

void Shape::LoadFromFile(const Json::Value& value,
						  const std::string& dir)
{
	m_name = value["name"].asString();
}

void Shape::StoreToFile(Json::Value& value,
						 const std::string& dir) const
{
	value["name"] = m_name;
	value["type"] = GetShapeDesc();
}

}