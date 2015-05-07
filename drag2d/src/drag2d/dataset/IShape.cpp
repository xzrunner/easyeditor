#include "IShape.h"

#include <wx/wx.h>

namespace d2d
{

IShape::IShape() 
{ 
	static int index = 0;
	name = "shape" + wxString::FromDouble(index++);
}

void IShape::LoadFromFile(const Json::Value& value,
						  const std::string& dir)
{
	name = value["name"].asString();
}

void IShape::StoreToFile(Json::Value& value,
						 const std::string& dir) const
{
	value["name"] = name;
}

} // d2d