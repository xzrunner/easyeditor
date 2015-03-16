#ifndef _EASYIMAGE_IMAGE_REVERT_Y_H_
#define _EASYIMAGE_IMAGE_REVERT_Y_H_

#include <drag2d.h>

namespace eimage
{

class ImageRevertY
{
public:
	ImageRevertY(const uint8_t* pixels, int width, int height);

	uint8_t* Revert();

private:
	const uint8_t* m_pixels;

	int m_width, m_height;

}; // ImageRevertY

}

#endif // _EASYIMAGE_IMAGE_REVERT_Y_H_