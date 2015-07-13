#include "ImageTrimRaw.h"

namespace eimage
{

ImageTrimRaw::ImageTrimRaw(const uint8_t* pixels, int w, int h)
	: m_pixels(pixels)
	, m_width(w)
	, m_height(h)
{
}

d2d::Rect ImageTrimRaw::Trim() const
{
	d2d::Rect sub;
	sub.makeInfinite();

	if (!m_pixels || m_width <= 0 || m_height <= 0) {
		return sub;
	}

	// down
	sub.yMin = 0;
	for (int i = 0; i < m_height; ++i)
	{
		int j = 0;
		for ( ; j < m_width; ++j) {
			if (!IsTransparent(j, i)) {
				break;
			}
		}
		if (j == m_width) {
			++sub.yMin;
		} else {
			break;
		}
	}
	// up
	sub.yMax = m_height;
	for (int i = m_height - 1; i >= 0; --i)
	{
		int j = 0;
		for ( ; j < m_width; ++j) {
			if (!IsTransparent(j, i)) {
				break;
			}
		}
		if (j == m_width) {
			--sub.yMax;
		} else {
			break;
		}
	}
	// left
	sub.xMin = 0;
	for (int i = 0; i < m_width; ++i)
	{
		int j = 0;
		for ( ; j < m_height; ++j) {
			if (!IsTransparent(i, j)) {
				break;
			}
		}
		if (j == m_height) {
			++sub.xMin;
		} else {
			break;
		}
	}
	// right
	sub.xMax = m_width;
	for (int i = m_width - 1; i >= 0; --i)
	{
		int j = 0;
		for ( ; j < m_height; ++j) {
			if (!IsTransparent(i, j)) {
				break;
			}
		}
		if (j == m_height) {
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

bool ImageTrimRaw::IsTransparent(int x, int y) const
{
	return m_pixels[(m_width * y + x) * 4 + 3] == 0;
}

}