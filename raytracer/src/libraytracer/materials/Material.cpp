#include "Material.h"

#include "utilities/Constants.h"

namespace rt
{

Material::Material()
{
}

Material::Material(const Material& m)
{
}

Material* Material::Clone() const
{
	return NULL;
}

RGBColor Material::AreaLightShade(const ShadeRec& sr) const
{
	return BLACK;
}

RGBColor Material::GetLe(const ShadeRec& sr) const
{
	return BLACK;
}

}