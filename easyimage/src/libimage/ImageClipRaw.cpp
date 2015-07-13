#include "ImageClipRaw.h"

namespace eimage
{

ImageClipRaw::ImageClipRaw(const uint8_t* pixels, int w, int h, int c,
						   bool check_duplicate)
	: m_pixels(pixels)
	, m_width(w)
	, m_height(h)
	, m_channels(c)
	, m_check(NULL)
{
	if (check_duplicate) {
		int sz = m_width * m_height;
		m_check = new bool[sz];
		memset(m_check, 0, sizeof(bool) * sz);
	}
}

ImageClipRaw::~ImageClipRaw()
{
	delete[] m_check;
}

const uint8_t* ImageClipRaw::Clip(const d2d::Rect& r) const
{
	return Clip(r.xMin, r.xMax, r.yMin, r.yMax);
}

const uint8_t* ImageClipRaw::Clip(int xmin, int xmax, int ymin, int ymax) const
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

	if (m_check) {
		memset(sub, 0, sz);
		for (int y = ymin; y < ymax; ++y) {
			for (int x = xmin; x < xmax; ++x) {
				if (x >= 0 && x < m_width &&
					y >= 0 && y < m_height) {
					int to = ((h - (y - ymin) - 1) * w + x-xmin) * m_channels;
					if (m_check[m_width * y + x]) {
						memset(&sub[to], 0, sizeof(uint8_t) * 4);
					} else {
						m_check[m_width * y + x] = true;
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
							to = ((h - (y - ymin) - 1) * w + x-xmin) * m_channels;
						memcpy(&sub[to], &m_pixels[from], m_channels);
				}
			}
		}
	}

	return sub;
}

}