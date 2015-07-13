#ifndef _EASYIMAGE_IMAGE_CLIP_RAW_H_
#define _EASYIMAGE_IMAGE_CLIP_RAW_H_

#include <drag2d.h>

namespace eimage
{

class ImageClipRaw
{
public:
	ImageClipRaw(const uint8_t* pixels, int w, int h, int c,
		bool check_duplicate = false);
	~ImageClipRaw();

	const uint8_t* Clip(const d2d::Rect& r) const;
	const uint8_t* Clip(int xmin, int xmax, int ymin, int ymax) const;

private:
	const uint8_t* m_pixels;
	int m_width, m_height;
	int m_channels;

	bool* m_check;

}; // ImageClipRaw

}

#endif // _EASYIMAGE_IMAGE_CLIP_RAW_H_