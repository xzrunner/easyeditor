#ifndef _EASYIMAGE_IMAGE_TRIM_RAW_H_
#define _EASYIMAGE_IMAGE_TRIM_RAW_H_

#include <drag2d.h>

namespace eimage
{

class ImageTrimRaw
{
public:
	ImageTrimRaw(const uint8_t* pixels, int w, int h);

	// remove transparent border
	// return: make to center
	d2d::Rect Trim() const;

private:
	bool IsTransparent(int x, int y) const;

private:
	const uint8_t* m_pixels;
	int m_width, m_height;

}; // ImageTrimRaw

}

#endif // _EASYIMAGE_IMAGE_TRIM_RAW_H_