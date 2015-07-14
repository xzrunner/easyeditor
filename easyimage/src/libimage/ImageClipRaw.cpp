#include "ImageClipRaw.h"

namespace eimage
{

ImageClipRaw::ImageClipRaw(const d2d::ImageData& img_data,
						   bool check_duplicate)
	: m_img_data(img_data)
	, m_check(NULL)
{
	if (check_duplicate) {
		int sz = m_img_data.GetWidth() * m_img_data.GetHeight();
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
	const uint8_t* pixels = m_img_data.GetPixelData();
	int width = m_img_data.GetWidth(),
		height = m_img_data.GetHeight();
	int channels = m_img_data.GetChannels();

	if (!pixels || width <= 0 || height <= 0) {
		return NULL;
	}

	if (xmin >= xmax || ymin >= ymax) {
		return NULL;
	}

	int w = xmax - xmin,
		h = ymax - ymin;
	int sz = w * h * channels;
	uint8_t* sub = new uint8_t[sz];
	if (!sub) {
		return NULL;
	}

	if (m_check) {
		memset(sub, 0, sz);
		for (int y = ymin; y < ymax; ++y) {
			for (int x = xmin; x < xmax; ++x) {
				if (x >= 0 && x < width &&
					y >= 0 && y < height) {
					int to = ((h - (y - ymin) - 1) * w + x-xmin) * channels;
					if (m_check[width * y + x]) {
						memset(&sub[to], 0, sizeof(uint8_t) * 4);
					} else {
						m_check[width * y + x] = true;
						int from = (y * width + x) * channels;
						memcpy(&sub[to], &pixels[from], channels);
					}
				}
			}
		}
	} else {
		memset(sub, 0, sz);
		for (int y = ymin; y < ymax; ++y) {
			for (int x = xmin; x < xmax; ++x) {
				if (x >= 0 && x < width &&
					y >= 0 && y < height) {
						int from = (y * width + x) * channels,
							to = ((h - (y - ymin) - 1) * w + x-xmin) * channels;
						memcpy(&sub[to], &pixels[from], channels);
				}
			}
		}
	}

	return sub;
}

}