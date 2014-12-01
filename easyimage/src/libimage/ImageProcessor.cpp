#include "ImageProcessor.h"

#include <drag2d.h>

//#define USE_SOIL

namespace eimage
{

ImageProcessor::ImageProcessor(d2d::Image* image)
	: m_image(image)
{	
}

const unsigned char* ImageProcessor::clip(int xmin, int xmax, int ymin, int ymax) const
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

// 	// todo y方向反转
// 	int _ymin = height - ymax;
// 	int _ymax = height - ymin;
// 	ymin = _ymin;
// 	ymax = _ymax;

	int w = xmax - xmin,
		h = ymax - ymin;
	int sz = w * h * channels;
	unsigned char* sub = new unsigned char[sz];
	if (!sub) {
		return NULL;
	}

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

//	// todo cp line
// 	int line_size = channels * w;
// 	for (int i = 0; i < h; ++i)
// 	{
// 		int from = (width * (ymin + i) + xmin) * channels,
// 			to = (h - 1 - i) * w * channels;
// 		memcpy(&sub[to], &pixels[from], line_size);
// 	}

	return sub;
}

d2d::Rect ImageProcessor::trim() const
{
	d2d::Rect sub;
	sub.makeInfinite();

	if (!m_image) {
		return sub;
	}

	int width = m_image->originWidth(),
		height = m_image->originHeight();
	int channels = m_image->channels();
	if (channels != 4 || width == 0 || height == 0) {
		return sub;
	}

	sub = m_image->getRegion();

	// down
	sub.yMin = 0;
	for (int i = 0; i < height; ++i)
	{
		int j = 0;
		for ( ; j < width; ++j) {
			if (!isTransparent(j, i)) {
				break;
			}
		}
		if (j == width) {
			++sub.yMin;
		} else {
			break;
		}
	}
	// up
	sub.yMax = height;
	for (int i = height - 1; i >= 0; --i)
	{
		int j = 0;
		for ( ; j < width; ++j) {
			if (!isTransparent(j, i)) {
				break;
			}
		}
		if (j == width) {
			--sub.yMax;
		} else {
			break;
		}
	}
	// left
	sub.xMin = 0;
	for (int i = 0; i < width; ++i)
	{
		int j = 0;
		for ( ; j < height; ++j) {
			if (!isTransparent(i, j)) {
				break;
			}
		}
		if (j == height) {
			++sub.xMin;
		} else {
			break;
		}
	}
	// right
	sub.xMax = width;
	for (int i = width - 1; i >= 0; --i)
	{
		int j = 0;
		for ( ; j < height; ++j) {
			if (!isTransparent(i, j)) {
				break;
			}
		}
		if (j == height) {
			--sub.xMax;
		} else {
			break;
		}
	}

	if (sub.isValid()) {
		return sub;
	} else {
		const d2d::Rect& r = m_image->getRegion();
		d2d::Rect ret;
		ret.xMin = ret.yMin = 0;
		ret.xMax = r.xLength();
		ret.yMax = r.yLength();
		return ret;
	}
}

bool ImageProcessor::isTransparent(int x, int y) const
{
	if (!m_image) {
		return false;
	}

	const unsigned char* pixels = m_image->getPixelData();
	if (!pixels) {
		return false;
	}

	int width = m_image->originWidth(),
		height = m_image->originHeight();
	int channels = m_image->channels();

#ifdef USE_SOIL
	// image data from top to bottom
	return pixels[(width * (height - y - 1) + x) * channels + channels - 1] == 0;
#else
	return pixels[(width * y + x) * channels + channels - 1] == 0;
#endif
}

}