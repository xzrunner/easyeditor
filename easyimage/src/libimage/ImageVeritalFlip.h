#ifndef _EASYIMAGE_IMAGE_VERITAL_FLIP_H_
#define _EASYIMAGE_IMAGE_VERITAL_FLIP_H_

#include <drag2d.h>

namespace eimage
{

class ImageVeritalFlip
{
public:
	ImageVeritalFlip(const uint8_t* pixels, int width, int height);

	uint8_t* Revert();

private:
	const uint8_t* m_pixels;

	int m_width, m_height;

}; // ImageVeritalFlip

}

#endif // _EASYIMAGE_IMAGE_VERITAL_FLIP_H_