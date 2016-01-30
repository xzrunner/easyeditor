#include "Shape.h"
#include "StringHelper.h"

namespace ee
{

Shape::Shape() 
{ 
	static int index = 0;
	name = "shape" + StringHelper::ToString(index++);
}

void Shape::LoadFromFile(const Json::Value& value,
						  const std::string& dir)
{
	name = value["name"].asString();
}

void Shape::StoreToFile(Json::Value& value,
						 const std::string& dir) const
{
	value["name"] = name;
	value["type"] = GetShapeDesc();
}

}