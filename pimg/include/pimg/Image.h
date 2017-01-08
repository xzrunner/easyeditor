#ifndef _PIMG_IMAGE_H_
#define _PIMG_IMAGE_H_

namespace pimg
{

class Image
{
public:
	uint8_t* pixels;
	int width, height;
	int channels;

	Image(uint8_t* pixels, int w, int h, int c)
		: pixels(pixels), width(w), height(h), channels(c) {}

}; // Image

}

#endif // _PIMG_IMAGE_H_