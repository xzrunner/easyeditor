#include "ImageTrim.h"

namespace eimage
{

ImageTrim::ImageTrim(const d2d::ImageData& img_data)
	: m_img_data(img_data)
{
}

d2d::Rect ImageTrim::Trim() const
{
	d2d::Rect sub;
	sub.makeInfinite();

	if (m_img_data.GetChannels() != 4) {
		return sub;
	}

	const uint8_t* pixels = m_img_data.GetPixelData();
	int width = m_img_data.GetWidth(),
		height = m_img_data.GetHeight();

	if (!pixels || width <= 0 || height <= 0) {
		return sub;
	}

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
	return m_img_data.GetPixelData()[(m_img_data.GetWidth() * y + x) * 4 + 3] == 0;
}

}