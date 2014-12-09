#include "ImageClip.h"

namespace eimage
{

ImageClip::ImageClip(const d2d::Image* image, bool check_duplicate)
	: m_image(image)
	, m_check(NULL)
{
	if (check_duplicate) {
		int sz = image->originWidth() * image->originHeight();
		m_check = new bool[sz];
		memset(m_check, 0, sizeof(bool) * sz);
	}
}

ImageClip::~ImageClip()
{
	delete[] m_check;
}

const byte* ImageClip::Clip(int xmin, int xmax, int ymin, int ymax) const
{
	if (!m_image) {
		return NULL;
	}

	int width = m_image->originWidth(),
		height = m_image->originHeight();
	int channels = m_image->channels();
	const unsigned char* pixels = m_image->getPixelData();

	if (xmin >= xmax || ymin >= ymax) {
		return NULL;
	}

	int w = xmax - xmin,
		h = ymax - ymin;
	int sz = w * h * channels;
	byte* sub = new byte[sz];
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
						memset(&sub[to], 0, sizeof(byte) * 4);
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