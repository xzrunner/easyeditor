#ifndef _RT_TEXTURE_H_
#define _RT_TEXTURE_H_

#include "utilities/RGBColor.h"
#include "utilities/Object.h"

namespace rt
{

class ShadeRec;

class Texture : public Object
{
public:
	virtual ~Texture() {}

	virtual RGBColor GetColor(const ShadeRec& sr) const = 0;

}; // Texture

}

#endif // _RT_TEXTURE_H_