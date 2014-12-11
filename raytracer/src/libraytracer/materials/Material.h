#ifndef _RT_MATERIALI_H_
#define _RT_MATERIALI_H_

#include "utilities/RGBColor.h"

namespace rt
{

class ShadeRec;

class Material
{
public:
	virtual ~Material() {}

	virtual RGBColor Shade(const ShadeRec& sr) const = 0;
	virtual RGBColor AreaLightShade(const ShadeRec& sr) const  = 0;

	virtual RGBColor GetLe(const ShadeRec& sr) const;

}; // Material

}

#endif // _RT_MATERIALI_H_