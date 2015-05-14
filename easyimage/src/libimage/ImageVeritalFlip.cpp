#include "ImageVeritalFlip.h"

namespace eimage
{

ImageVeritalFlip::ImageVeritalFlip(const uint8_t* pixels, int width, int height)
	: m_pixels(pixels)
	, m_width(width)
	, m_height(height)
{
}

uint8_t* ImageVeritalFlip::Revert()
{
	int sz = m_width * m_height * 4;
	uint8_t* pixels = new uint8_t[sz];
	
	int line_sz = m_width * 4;
	for (int y = 0; y < m_height; ++y) {
		memcpy(&pixels[(m_height - 1 - y) * line_sz], &m_pixels[y * line_sz], line_sz);
	}

	return pixels;
}

}