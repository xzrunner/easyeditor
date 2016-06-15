#include "PixelUncoveredLUT.h"

#include <assert.h>

namespace eimage
{

PixelUncoveredLUT::PixelUncoveredLUT(int width, int height)
	: m_width(width)
	, m_height(height)
{
	int sz = m_width * m_height;

	m_covered_count = new int[sz];
	m_empty_area = new int[sz];
	m_multi_area = new int[sz];

	m_const_count = new int[sz];
	m_const_area = new int[sz];
}

PixelUncoveredLUT::~PixelUncoveredLUT()
{
	delete[] m_covered_count;
	delete[] m_empty_area;
	delete[] m_multi_area;

	delete[] m_const_count;
	delete[] m_const_area;
}

int PixelUncoveredLUT::GetUncoveredArea(int x, int y, int w, int h) const
{
	return GetArea(m_empty_area, x, y, w, h);
}

int PixelUncoveredLUT::GetMultiArea(int x, int y, int w, int h) const
{
	return GetArea(m_multi_area, x, y, w, h);	
}

bool PixelUncoveredLUT::IntersectConstArea(int x, int y, int w, int h) const
{
	return GetArea(m_const_area, x, y, w, h) != 0;
}

void PixelUncoveredLUT::LoadRects(const std::vector<Rect>& rects)
{
	int sz = m_width * m_height;

	memset(m_covered_count, 0, sizeof(int) * sz);
	for (int i = 0, n = rects.size(); i < n; ++i) {
		const Rect& r = rects[i];
		for (int x = r.x; x < r.x + r.w; ++x) {
			assert(x >= 0 && x < m_width);
			for (int y = r.y; y < r.y + r.h; ++y) {
				assert(y >= 0 && y < m_height);
				++m_covered_count[y * m_width + x];
			}
		}
	}

	memset(m_empty_area, 0, sizeof(int) * sz);
	int curr_idx = 0;
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			int left = x > 0 ? m_empty_area[curr_idx-1] : 0;
			int down = y > 0 ? m_empty_area[curr_idx-m_width] : 0;
			int left_down = (x > 0 && y > 0) ? m_empty_area[curr_idx-m_width-1] : 0;
			int curr = left + down - left_down;
			if (m_covered_count[curr_idx] == 0) {
				++curr;
			}
			m_empty_area[curr_idx] = curr;

			++curr_idx;
		}
	}

	memset(m_multi_area, 0, sizeof(int) * sz);
	curr_idx = 0;
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			int left = x > 0 ? m_multi_area[curr_idx-1] : 0;
			int down = y > 0 ? m_multi_area[curr_idx-m_width] : 0;
			int left_down = (x > 0 && y > 0) ? m_multi_area[curr_idx-m_width-1] : 0;
			int curr = left + down - left_down;
			if (m_covered_count[curr_idx] > 1) {
				curr += m_covered_count[curr_idx] - 1;
			}
			m_multi_area[curr_idx] = curr;

			++curr_idx;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	memset(m_const_count, 0, sizeof(int) * sz);
	for (int i = 0, n = rects.size(); i < n; ++i) {
		const Rect& r = rects[i];
		if (!r.is_const) {
			continue;
		}
		for (int x = r.x; x < r.x + r.w; ++x) {
			assert(x >= 0 && x < m_width);
			for (int y = r.y; y < r.y + r.h; ++y) {
				assert(y >= 0 && y < m_height);
				++m_const_count[y * m_width + x];
			}
		}
	}

	memset(m_const_area, 0, sizeof(int) * sz);
	curr_idx = 0;
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			int left = x > 0 ? m_const_area[curr_idx-1] : 0;
			int down = y > 0 ? m_const_area[curr_idx-m_width] : 0;
			int left_down = (x > 0 && y > 0) ? m_const_area[curr_idx-m_width-1] : 0;
			int curr = left + down - left_down;
			if (m_const_count[curr_idx] != 0) {
				++curr;
			}
			m_const_area[curr_idx] = curr;

			++curr_idx;
		}
	}
}

int PixelUncoveredLUT::GetArea(int* area, int x, int y, int w, int h) const
{
	assert(w >= 1 && h >= 1
		&& x >= 0 && x + w <= m_width
		&& y >= 0 && y + h <= m_height);
	int left_down_area = (x > 0 && y > 0) ? area[(y-1)*m_width+x-1] : 0;
	int left_area = x > 0 ? area[(y-1+h)*m_width+x-1] : 0;
	int right_area = y > 0 ? area[(y-1)*m_width+x-1+w] : 0;
	int right_up_area = area[(y-1+h)*m_width+x-1+w];

	int a = right_up_area - left_area - right_area + left_down_area;
	assert(a >= 0 && a <= area[m_width*m_height-1]);
	return a;
}

}