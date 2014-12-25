#include "ImageTrim.h"

//#define USE_SOIL

namespace eimage
{

ImageTrim::ImageTrim(const d2d::Image* image)
	: m_image(image)
{
}

d2d::Rect ImageTrim::Trim() const
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
			if (!IsTransparent(j, i)) {
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
			if (!IsTransparent(j, i)) {
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
			if (!IsTransparent(i, j)) {
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
			if (!IsTransparent(i, j)) {
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
		d2d::Rect ret;

// 		const d2d::Rect& r = m_image->getRegion();
// 		ret.xMin = ret.yMin = 0;
// 		ret.xMax = r.xLength();
// 		ret.yMax = r.yLength();

		ret.xMin = ret.yMin = 0;
		ret.xMax = ret.yMax = 1;

		return ret;
	}
}

bool ImageTrim::IsTransparent(int x, int y) const
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