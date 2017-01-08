#include "Condense.h"
#include "Cropping.h"

namespace pimg
{

Condense::Condense(const uint8_t* pixels, int width, int height)
	: m_pixels(pixels)
	, m_width(width)
	, m_height(height)
{	
}

Rect Condense::GetRegion() const
{
	Rect region;

	if (!m_pixels || m_width <= 0 || m_height <= 0) {
		return region;
	}

	// down
	region.ymin = 0;
	for (int i = 0; i < m_height; ++i)
	{
		int j = 0;
		for ( ; j < m_width; ++j) {
			if (!IsTransparent(j, i)) {
				break;
			}
		}
		if (j == m_width) {
			++region.ymin;
		} else {
			break;
		}
	}
	// up
	region.ymax = m_height;
	for (int i = m_height - 1; i >= 0; --i)
	{
		int j = 0;
		for ( ; j < m_width; ++j) {
			if (!IsTransparent(j, i)) {
				break;
			}
		}
		if (j == m_width) {
			--region.ymax;
		} else {
			break;
		}
	}
	// left
	region.xmin = 0;
	for (int i = 0; i < m_width; ++i)
	{
		int j = 0;
		for ( ; j < m_height; ++j) {
			if (!IsTransparent(i, j)) {
				break;
			}
		}
		if (j == m_height) {
			++region.xmin;
		} else {
			break;
		}
	}
	// right
	region.xmax = m_width;
	for (int i = m_width - 1; i >= 0; --i)
	{
		int j = 0;
		for ( ; j < m_height; ++j) {
			if (!IsTransparent(i, j)) {
				break;
			}
		}
		if (j == m_height) {
			--region.xmax;
		} else {
			break;
		}
	}

	return region;
}

uint8_t* Condense::GetPixels(Rect& region) const
{
	region = GetRegion();
	if (!region.IsValid()) {
		return NULL;
	}

	Cropping crop(m_pixels, m_width, m_height, 4);
	return crop.Crop(region);
}

bool Condense::IsTransparent(int x, int y) const
{
	return m_pixels[(y * m_width + x) * 4 + 3] == 0;
}

}