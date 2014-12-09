#ifndef _EASYIMAGE_IMAGE_CLIP_H_
#define _EASYIMAGE_IMAGE_CLIP_H_

#include <drag2d.h>

namespace eimage
{

class ImageClip
{
public:
	ImageClip(const d2d::Image* image, bool check_duplicate = false);
	~ImageClip();

	const byte* Clip(int xmin, int xmax, int ymin, int ymax) const;

private:
	const d2d::Image* m_image;

	bool* m_check;

}; // ImageClip

}

#endif // _EASYIMAGE_IMAGE_CLIP_H_