#ifndef _GUM_IMAGE_LOADER_H_
#define _GUM_IMAGE_LOADER_H_

#include <stdint.h>

namespace gum
{

class ImageLoader
{
public:
	static void FormatPixelsAlpha(uint8_t* pixels, int width, int height, int val);

	static void PreMuiltiAlpha(uint8_t* pixels, int width, int height);

	static void RemoveGhostPixel(uint8_t* pixels, int width, int height);

}; // ImageLoader

}

#endif // _GUM_IMAGE_LOADER_H_
