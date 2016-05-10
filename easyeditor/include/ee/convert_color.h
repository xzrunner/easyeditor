#ifndef _EASYEDITOR_CONVERT_COLOR_H_
#define _EASYEDITOR_CONVERT_COLOR_H_

namespace ee
{

void hsl2rgb(float h, float s, float l, float& r, float& g, float& b);
void rgb2hsl(float r, float g, float b, float& h, float& s, float& l);

}

#endif // _EASYEDITOR_CONVERT_COLOR_H_