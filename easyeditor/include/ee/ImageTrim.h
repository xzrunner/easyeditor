#ifndef _EASYIMAGE_IMAGE_TRIM_H_
#define _EASYIMAGE_IMAGE_TRIM_H_

#include <drag2d.h>

namespace eimage
{

class ImageTrim
{
public:
	ImageTrim(const ImageData& img_data);

	// remove transparent border
	// return: make to center
	Rect Trim() const;

private:
	bool IsTransparent(int x, int y) const;

private:
	const ImageData& m_img_data;

}; // ImageTrim

}

#endif // _EASYIMAGE_IMAGE_TRIM_H_