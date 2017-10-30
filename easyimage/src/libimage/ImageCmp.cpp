#include "ImageCmp.h"

#include <gimg_typedef.h>
#include <gimg_import.h>

namespace eimage
{

bool ImageCmp::IsSame(const std::string& path0, const std::string& path1, float val)
{
	int w0, h0, fmt0;
	uint8_t* pixels0 = gimg_import(path0.c_str(), &w0, &h0, &fmt0);

	int w1, h1, fmt1;
	uint8_t* pixels1 = gimg_import(path1.c_str(), &w1, &h1, &fmt1);

	if (!pixels0 || !pixels1 ||
		fmt0 != GPF_RGBA8 || fmt1 != GPF_RGBA8) {
		return false;
	}

	int tot_count = 0, same_count = 0;
	for (int y = 0; y < h0; ++y) 
	{
		if (y >= h1) {
			break;
		}
		for (int x = 0; x < w0; ++x) 
		{
			if (x >= w1) {
				break;
			}
			if (IsPixelTransparente(pixels0, w0, h0, x, y) &&
				IsPixelTransparente(pixels1, w1, h1, x, y)) {
				continue;
			}
			++tot_count;
			if (memcmp(&pixels0[(y*w0+x)*4+3], &pixels1[(y*w1+x)*4+3], sizeof(uint8_t) * 4) == 0) {
				++same_count;
			}
		}
	}

	return static_cast<float>(same_count) / tot_count > val;
}

bool ImageCmp::IsPixelTransparente(uint8_t* pixels, int w, int h, int x, int y)
{
	if (x < 0 || x >= w ||
		y < 0 || y >= h) {
		return true;
	}

	uint8_t alpha = pixels[(y*w+x)*4+3];
	return alpha == 0;
}

}