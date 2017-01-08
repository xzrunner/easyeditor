#include "Cropping.h"
#include "Image.h"
#include "Rect.h"

#include <string>

namespace pimg
{

Cropping::Cropping(const uint8_t* pixels, int width, int height, int channels, bool check_duplicate)
	: m_pixels(pixels)
	, m_width(width)
	, m_height(height)
	, m_channels(channels)
	, m_flags(NULL)
{
	if (check_duplicate) 
	{
		int sz = m_width * m_height;
		m_flags = new bool[sz];
		memset(m_flags, 0, sizeof(bool) * sz);
	}
}

Cropping::~Cropping()
{
	if (m_flags) {
		delete[] m_flags;
	}
}

uint8_t* Cropping::Crop(int xmin, int ymin, int xmax, int ymax) const
{
	if (!m_pixels || m_width <= 0 || m_height <= 0) {
		return NULL;
	}
	if (xmin >= xmax || ymin >= ymax) {
		return NULL;
	}

	int w = xmax - xmin,
		h = ymax - ymin;
	int sz = w * h * m_channels;
	uint8_t* sub = new uint8_t[sz];
	if (!sub) {
		return NULL;
	}

	if (m_flags) {
		memset(sub, 0, sz);
		for (int y = ymin; y < ymax; ++y) {
			for (int x = xmin; x < xmax; ++x) {
				if (x >= 0 && x < m_width &&
					y >= 0 && y < m_height) {
						int to = ((y - ymin) * w + x-xmin) * m_channels;
						if (m_flags[m_width * y + x]) {
							memset(&sub[to], 0, sizeof(uint8_t) * 4);
						} else {
							m_flags[m_width * y + x] = true;
							int from = (y * m_width + x) * m_channels;
							memcpy(&sub[to], &m_pixels[from], m_channels);
						}
				}
			}
		}
	} else {
		memset(sub, 0, sz);
		for (int y = ymin; y < ymax; ++y) {
			for (int x = xmin; x < xmax; ++x) {
				if (x >= 0 && x < m_width &&
					y >= 0 && y < m_height) {
						int from = (y * m_width + x) * m_channels,
							to = ((y - ymin) * w + x-xmin) * m_channels;
						memcpy(&sub[to], &m_pixels[from], m_channels);
				}
			}
		}
	}

	return sub;
}

uint8_t* Cropping::Crop(const Rect& r) const
{
	return Crop(r.xmin, r.ymin, r.xmax, r.ymax);
}

}