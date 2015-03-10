#ifndef _DRAG2D_COLOR_CONVERT_H_
#define _DRAG2D_COLOR_CONVERT_H_

#include "Color.h"

namespace d2d
{

Colori hsl2rgb(float h, float s, float l);
Colori rgb2hsl(float r, float g, float b);

}

#endif // _DRAG2D_COLOR_CONVERT_H_