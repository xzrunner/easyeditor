#include "ExtractOutlineRaw.h"

#include <ee/Image.h>
#include <ee/Math2D.h>

#include <SM_ConvexHull.h>
#include <SM_DouglasPeucker.h>

#include <queue>

#include <assert.h>

namespace eimage
{

// order
// 3 4 5
// 2   6
// 1 0 7
static const int NEARBY_COUNT = 8;
static const sm::vec2 NEARBY_OFFSET[NEARBY_COUNT] = {
	sm::vec2( 0, -1),
	sm::vec2(-1, -1),
	sm::vec2(-1,  0),
	sm::vec2(-1,  1),
	sm::vec2( 0,  1),
	sm::vec2( 1,  1),
	sm::vec2( 1,  0),
	sm::vec2( 1, -1),
};

//////////////////////////////////////////////////////////////////////////
//// 四联通
//////////////////////////////////////////////////////////////////////////
//// order
////   2
//// 1   3
////   0
//static const int QUERY_COUNT = 4;
//static const sm::vec2 QUERY_OFFSET[NEARBY_COUNT] = {
//	sm::vec2( 0, -1),
//	sm::vec2(-1,  0),
//	sm::vec2( 0,  1),
//	sm::vec2( 1,  0),
//};

//////////////////////////////////////////////////////////////////////////
// 八联通
//////////////////////////////////////////////////////////////////////////
// order
// 3 4 5
// 2   6
// 1 0 7
static const int QUERY_COUNT = 8;
static const sm::vec2 QUERY_OFFSET[NEARBY_COUNT] = {
	sm::vec2( 0, -1),
	sm::vec2(-1, -1),
	sm::vec2(-1,  0),
	sm::vec2(-1,  1),
	sm::vec2( 0,  1),
	sm::vec2( 1,  1),
	sm::vec2( 1,  0),
	sm::vec2( 1, -1),
};

ExtractOutlineRaw::ExtractOutlineRaw(const uint8_t* pixels, int w, int h)
	: m_pixels(pixels)
	, m_width(w)
	, m_height(h)
{
}

void ExtractOutlineRaw::CreateBorderPoints()
{
	m_border_points.clear();

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			bool is_border = IsPixelBorder(x, y);
			if (is_border) {
				m_border_points.push_back(sm::vec2(x+0.5f, y+0.5f));
			}
		}
	}
}

void ExtractOutlineRaw::CreateBorderLineAndMerge()
{
	CreateBorderLine();	
	MergeBorderLine();
}

void ExtractOutlineRaw::CreateBorderConvexHull()
{
	sm::convex_hull(m_border_line, m_convex_hull);
}

void ExtractOutlineRaw::CreateBorderLine()
{
	m_border_line.clear();

	// find start, downmost and leftmost
	bool* flag = new bool[m_width * m_height];
	sm::vec2 first;
	first.MakeInvalid();
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			bool is_border = IsPixelBorder(x, y);
			flag[m_width*y+x] = is_border;
			if (is_border) {
				if (first.IsValid()) {
					if (y < first.y || y == first.y && x < first.x) {
						first.Set(static_cast<float>(x), static_cast<float>(y));
					}
				} else {
					first.Set(static_cast<float>(x), static_cast<float>(y));
				}
			}
		}
	}

	sm::vec2 curr_pos = first;
	bool curr_valid = first.IsValid();
	int curr_dir = -1;
	while (curr_valid) 
	{
		// finish
		if (!m_border_line.empty() &&
			curr_pos == first) {
			m_border_axis_line.push_back(curr_pos + sm::vec2(0.5f, 0.5f));
			break;
		}
		m_border_axis_line.push_back(curr_pos + sm::vec2(0.5f, 0.5f));

		int next_dir;
		if (curr_dir == -1) {
			// the first, start from [0]
			next_dir = 0;
		} else {
			// curr dir is next of inverse last dir
			next_dir = (curr_dir + (QUERY_COUNT >> 1)) + 1;
		}
		// search in certain order
		for (int i = 0; i < QUERY_COUNT; ++i) {
			int dir = (i+next_dir)%QUERY_COUNT;
			sm::vec2 nearby = curr_pos + QUERY_OFFSET[dir];
			// connect
			if (!IsPixelTransparente(flag, static_cast<int>(nearby.x), static_cast<int>(nearby.y))) {
				curr_dir = dir;
				curr_pos = nearby;
				curr_valid = true;
				break;
			} else {
				sm::vec2 new_pos = curr_pos;
				if (dir == 0) {
					new_pos.x += 0.5f;
				} else if (dir == 1) {
					;
				} else if (dir == 2) {
					new_pos.y += 0.5f;
				} else if (dir == 3) {
					new_pos.y += 1;
				} else if (dir == 4) {
					new_pos.x += 0.5f;
					new_pos.y += 1;
				} else if (dir == 5) {
					new_pos.x += 1;
					new_pos.y += 1;
				} else if (dir == 6) {
					new_pos.x += 1;
					new_pos.y += 0.5f;
				} else {
					assert(dir == 7);
					new_pos.x += 1;
				}
				if (m_border_line.empty() || m_border_line.back() != new_pos) {
					m_border_line.push_back(new_pos);
				}
			}
		}	
	}

	delete[] flag;
}

void ExtractOutlineRaw::MergeBorderLine()
{
	m_border_line_merged.clear();
	//	MergeRawBorder(border, border_merged);
	sm::douglas_peucker(m_border_line, 1.5f, m_border_line_merged);
}

bool ExtractOutlineRaw::IsPixelBorder(int x, int y) const
{
	if (x < 0 || x >= m_width ||
		y < 0 || y >= m_height) {
		return false;
	}

	if (IsPixelTransparente(x, y)) {
		return false;
	}

	for (int i = 0; i < NEARBY_COUNT; ++i) {
		if (IsPixelTransparente(static_cast<int>(x+NEARBY_OFFSET[i].x), 
			static_cast<int>(y+NEARBY_OFFSET[i].y))) {
			return true;
		}
	}
	return false;
}

bool ExtractOutlineRaw::IsPixelTransparente(int x, int y) const
{
	if (x < 0 || x >= m_width ||
		y < 0 || y >= m_height) {
		return true;
	}
	
	uint8_t alpha = m_pixels[(y*m_width+x)*4+3];
	return alpha == 0;
}

bool ExtractOutlineRaw::IsPixelTransparente(bool* flag, int x, int y) const
{
	if (x < 0 || x >= m_width ||
		y < 0 || y >= m_height) {
		return true;
	}

	return !flag[y*m_width+x];
}

bool ExtractOutlineRaw::IsNearby(const sm::vec2& p0, const sm::vec2& p1) const
{
	return abs(p0.x - p1.x) <= 1 && abs(p0.y - p1.y) <= 1;
}

void ExtractOutlineRaw::MergeLine(const std::vector<sm::vec2>& src, std::vector<sm::vec2>& dst)
{
	if (src.size() < 3) {
		return;
	}
	dst.clear();
	dst.push_back(src[0]);
	dst.push_back(src[1]);
	for (int i = 2, n = src.size(); i < n; ++i)
	{
		const sm::vec2& p = src[i];
		if (ee::Math2D::IsTwoLineParallel(dst[dst.size()-1], dst[dst.size()-2], dst[dst.size()-1], p)) {
			dst.pop_back();
		}
		dst.push_back(p);
	}
}

}