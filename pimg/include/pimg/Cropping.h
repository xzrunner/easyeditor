#ifndef _PIMG_CROPPING_H_
#define _PIMG_CROPPING_H_

#include <stdint.h>

namespace pimg
{

class Rect;

class Cropping
{
public:
	Cropping(const uint8_t* pixels, int width, int height, int channels, bool check_duplicate = false);
	~Cropping();

	uint8_t* Crop(int xmin, int ymin, int xmax, int ymax) const;
	uint8_t* Crop(const Rect& r) const;

private:
	const uint8_t* m_pixels;

	int m_width, m_height;
	int m_channels;

	bool* m_flags;

}; // Cropping

}

#endif // _PIMG_CROPPING_H_