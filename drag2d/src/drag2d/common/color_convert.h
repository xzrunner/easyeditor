#ifndef _DRAG2D_COLOR_CONVERT_H_
#define _DRAG2D_COLOR_CONVERT_H_

#include "Color.h"

namespace d2d
{

Colorf hsl2rgb(float h, float s, float l);
Colorf rgb2hsl(float r, float g, float b);

}

#endif // _DRAG2D_COLOR_CONVERT_H_