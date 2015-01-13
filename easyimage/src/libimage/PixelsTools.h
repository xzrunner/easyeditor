#ifndef _EASYIMAGE_PIXELS_TOOLS_H_
#define _EASYIMAGE_PIXELS_TOOLS_H_

typedef unsigned char uint8_t;

namespace eimage
{

uint8_t* RGBA2RGB(const uint8_t* rgba, int width, int height, bool invert_y = false);

}

#endif // _EASYIMAGE_PIXELS_TOOLS_H_
