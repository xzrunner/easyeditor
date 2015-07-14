#ifndef _EASYIMAGE_IMAGE_CLIP_RAW_H_
#define _EASYIMAGE_IMAGE_CLIP_RAW_H_

#include <drag2d.h>

namespace eimage
{

class ImageClipRaw
{
public:
	ImageClipRaw(const d2d::ImageData& img_data, bool check_duplicate = false);
	~ImageClipRaw();

	const uint8_t* Clip(const d2d::Rect& r) const;
	const uint8_t* Clip(int xmin, int xmax, int ymin, int ymax) const;

private:
	const d2d::ImageData& m_img_data;

	bool* m_check;

}; // ImageClipRaw

}

#endif // _EASYIMAGE_IMAGE_CLIP_RAW_H_