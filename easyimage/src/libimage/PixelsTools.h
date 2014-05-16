#ifndef _EASYIMAGE_PIXELS_TOOLS_H_
#define _EASYIMAGE_PIXELS_TOOLS_H_

typedef unsigned char byte;

namespace eimage
{

byte* RGBA2RGB(const byte* rgba, int width, int height, bool invert_y = false);

}

#endif // _EASYIMAGE_PIXELS_TOOLS_H_
