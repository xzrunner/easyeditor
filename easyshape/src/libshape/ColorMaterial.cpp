#include "ColorMaterial.h"

namespace eshape
{

ColorMaterial::ColorMaterial(const s2::Color& color)
	: s2::ColorPolygon(color)
{	
}

Json::Value ColorMaterial::Store(const std::string& dirpath) const
{
	Json::Value val;
	val["type"] = "color";
	val["color"] = m_color.ToRGBA();
	return val;
}

}