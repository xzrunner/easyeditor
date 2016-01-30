#ifndef _EASYEDITOR_CONVERT_COLOR_H_
#define _EASYEDITOR_CONVERT_COLOR_H_

#include "Color.h"

namespace ee
{

Colorf hsl2rgb(float h, float s, float l);
Colorf rgb2hsl(float r, float g, float b);

}

#endif // _EASYEDITOR_CONVERT_COLOR_H_