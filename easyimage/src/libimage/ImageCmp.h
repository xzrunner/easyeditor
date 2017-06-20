#ifndef _EASYIMAGE_IMAGE_CMP_H_
#define _EASYIMAGE_IMAGE_CMP_H_

#include <string>

#include <stdint.h>

namespace eimage
{

class ImageCmp
{
public:
	static bool IsSame(const std::string& path0, const std::string& path1, float val);

private:
	static bool IsPixelTransparente(uint8_t* pixels, int w, int h, int x, int y);

}; // ImageCmp

}

#endif // _EASYIMAGE_IMAGE_CMP_H_