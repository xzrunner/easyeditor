#ifndef _EASYSHAPE_COLOR_MATERIAL_H_
#define _EASYSHAPE_COLOR_MATERIAL_H_

#include "Material.h"

#include <sprite2/ColorPolygon.h>

namespace eshape
{

class ColorMaterial : public Material, public s2::ColorPolygon
{
public:
	ColorMaterial() {}
	ColorMaterial(const s2::Color& color);

	virtual Json::Value Store(const std::string& dirpath) const;

}; // ColorMaterial

}

#endif // _EASYSHAPE_COLOR_MATERIAL_H_