#include "ImageTrim.h"
#include "ImageData.h"

#include <gimg_typedef.h>

namespace ee
{

ImageTrim::ImageTrim(const ImageData& img_data)
	: m_img_data(img_data)
{
}

sm::rect ImageTrim::Trim() const
{
	sm::rect sub;
	sub.MakeEmpty();

	if (m_img_data.GetFormat() != GPF_RGBA) {
		return sub;
	}

	const uint8_t* pixels = m_img_data.GetPixelData();
	int width = m_img_data.GetWidth(),
		height = m_img_data.GetHeight();

	if (!pixels || width <= 0 || height <= 0) {
		return sub;
	}

	// down
	sub.ymin = 0;
	for (int i = 0; i < height; ++i)
	{
		int j = 0;
		for ( ; j < width; ++j) {
			if (!IsTransparent(j, i)) {
				break;
			}
		}
		if (j == width) {
			++sub.ymin;
		} else {
			break;
		}
	}
	// up
	sub.ymax = static_cast<float>(height);
	for (int i = height - 1; i >= 0; --i)
	{
		int j = 0;
		for ( ; j < width; ++j) {
			if (!IsTransparent(j, i)) {
				break;
			}
		}
		if (j == width) {
			--sub.ymax;
		} else {
			break;
		}
	}
	// left
	sub.xmin = 0;
	for (int i = 0; i < width; ++i)
	{
		int j = 0;
		for ( ; j < height; ++j) {
			if (!IsTransparent(i, j)) {
				break;
			}
		}
		if (j == height) {
			++sub.xmin;
		} else {
			break;
		}
	}
	// right
	sub.xmax = static_cast<float>(width);
	for (int i = width - 1; i >= 0; --i)
	{
		int j = 0;
		for ( ; j < height; ++j) {
			if (!IsTransparent(i, j)) {
				break;
			}
		}
		if (j == height) {
			--sub.xmax;
		} else {
			break;
		}
	}

	if (sub.IsValid()) {
		return sub;
	} else {
		sm::rect ret;

// 		const sm::rect& r = m_image->getRegion();
// 		ret.xmin = ret.ymin = 0;
// 		ret.xmax = r.Width();
// 		ret.ymax = r.Height();

		ret.xmin = ret.ymin = 0;
		ret.xmax = ret.ymax = 1;

		return ret;
	}
}

bool ImageTrim::IsTransparent(int x, int y) const
{
	return m_img_data.GetPixelData()[(y * m_img_data.GetWidth() + x) * 4 + 3] == 0;
}

}