#ifndef _RT_MATERIALI_H_
#define _RT_MATERIALI_H_

#include "utilities/RGBColor.h"

namespace rt
{

class ShadeRec;

class Material
{
public:

	virtual RGBColor Shade(const ShadeRec& sr) const = 0;

}; // Material

}

#endif // _RT_MATERIALI_H_