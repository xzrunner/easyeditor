#ifndef _EASYIMAGE_IMAGE_TRIM_H_
#define _EASYIMAGE_IMAGE_TRIM_H_

#include <drag2d.h>

namespace eimage
{

class ImageTrim
{
public:
	ImageTrim(const d2d::ImageData& img_data);

	// remove transparent border
	// return: make to center
	d2d::Rect Trim() const;

private:
	bool IsTransparent(int x, int y) const;

private:
	const d2d::ImageData& m_img_data;

}; // ImageTrim

}

#endif // _EASYIMAGE_IMAGE_TRIM_H_