#include "Material.h"

#include "utilities/Constants.h"

namespace rt
{

RGBColor Material::AreaLightShade(const ShadeRec& sr) const
{
	return BLACK;
}

RGBColor Material::GetLe(const ShadeRec& sr) const
{
	return BLACK;
}

}