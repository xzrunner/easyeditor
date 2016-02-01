#include "PixelAreaLUT.h"

#include <cmath>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

namespace eimage
{

PixelAreaLUT::PixelAreaLUT(const bool* pixels, int width, int height, bool to_block4)
	: m_width(width)
	, m_height(height)
{
	int sz = m_width*m_height;
	m_pixels = new bool[sz];
	memcpy(m_pixels, pixels, sizeof(bool)*sz);

	if (to_block4) {
		FixPixelsToBlock4();
	}

	LoadFromPixels();
}

PixelAreaLUT::~PixelAreaLUT()
{
	delete[] m_pixels;
	delete[] m_area;
}

int PixelAreaLUT::GetRectArea(int x, int y, int w, int h) const
{
	assert(w >= 1 && h >= 1
		&& x >= 0 && x + w <= m_width
		&& y >= 0 && y + h <= m_height);
	int left_down_area = (x > 0 && y > 0) ? m_area[(y-1)*m_width+x-1] : 0;
	int left_area = x > 0 ? m_area[(y-1+h)*m_width+x-1] : 0;
	int right_area = y > 0 ? m_area[(y-1)*m_width+x-1+w] : 0;
	int right_up_area = m_area[(y-1+h)*m_width+x-1+w];

	int area = right_up_area - left_area - right_area + left_down_area;
	assert(area >= 0 && area <= m_area[m_width*m_height-1]);
	return area;
}

void PixelAreaLUT::CutByRect(int _x, int _y, int _w, int _h, int& left_area)
{
	int old = m_area[m_width*m_height-1];

	for (int y = _y; y < m_height; ++y) {
		for (int x = _x; x < m_width; ++x) {
			int curr_idx = y * m_width + x;
			
			int left = x > 0 ? m_area[curr_idx-1] : 0;
			int down = y > 0 ? m_area[curr_idx-m_width] : 0;
			int left_down = (x > 0 && y > 0) ? m_area[curr_idx-m_width-1] : 0;
			int curr = left + down - left_down;

			if (x < _x + _w && y < _y + _h) {
				m_pixels[curr_idx] = false;
			}
			if (m_pixels[curr_idx]) {
				++curr;
			}

 			assert(m_area[curr_idx] >= curr);
			m_area[curr_idx] = curr;
		}
	}

	left_area = m_area[m_width*m_height-1];
}

void PixelAreaLUT::FixPixelsToBlock4()
{
	int bw = static_cast<int>(std::ceil(m_width / 4.0f)),
		bh = static_cast<int>(std::ceil(m_height / 4.0f));
	bool* bpixels = new bool[bw * bh];
	memset(bpixels, 0, sizeof(bool) * bw * bh);
	
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			if (m_pixels[y * m_width + x]) {
				bpixels[(y / 4) * bw + (x / 4)] = true;
			}
		}
	}

	memset(m_pixels, 0, sizeof(bool) * m_width * m_height);
	for (int by = 0; by < bh; ++by) {
		for (int bx = 0; bx < bw; ++bx) {
			if (bpixels[by * bw + bx]) {
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < 4; ++j) {
						int x = bx * 4 + i,
							y = by * 4 + j;
						if (x < m_width && y < m_height) {
							m_pixels[y * m_width + x] = true;
						}
					}
				}
			}
		}
	}

	delete[] bpixels;
}

void PixelAreaLUT::LoadFromPixels()
{
	int sz = m_width * m_height;
	m_area = new int[sz];
	memset(m_area, 0, sizeof(int) * sz);
	int curr_idx = 0;
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			int left = x > 0 ? m_area[curr_idx-1] : 0;
			int down = y > 0 ? m_area[curr_idx-m_width] : 0;
			int left_down = (x > 0 && y > 0) ? m_area[curr_idx-m_width-1] : 0;
			int curr = left + down - left_down;
			if (m_pixels[curr_idx]) {
				++curr;
			}
			m_area[curr_idx] = curr;

			++curr_idx;
		}
	}
}

}