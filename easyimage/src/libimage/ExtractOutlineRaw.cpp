#include "ExtractOutlineRaw.h"

#include <queue>

namespace eimage
{

// order
// 3 4 5
// 2   6
// 1 0 7
static const int NEARBY_COUNT = 8;
static const d2d::Vector NEARBY_OFFSET[NEARBY_COUNT] = {
	d2d::Vector( 0, -1),
	d2d::Vector(-1, -1),
	d2d::Vector(-1,  0),
	d2d::Vector(-1,  1),
	d2d::Vector( 0,  1),
	d2d::Vector( 1,  1),
	d2d::Vector( 1,  0),
	d2d::Vector( 1, -1),
};

//////////////////////////////////////////////////////////////////////////
//// 四联通
//////////////////////////////////////////////////////////////////////////
//// order
////   2
//// 1   3
////   0
//static const int QUERY_COUNT = 4;
//static const d2d::Vector QUERY_OFFSET[NEARBY_COUNT] = {
//	d2d::Vector( 0, -1),
//	d2d::Vector(-1,  0),
//	d2d::Vector( 0,  1),
//	d2d::Vector( 1,  0),
//};

//////////////////////////////////////////////////////////////////////////
// 八联通
//////////////////////////////////////////////////////////////////////////
// order
// 3 4 5
// 2   6
// 1 0 7
static const int QUERY_COUNT = 8;
static const d2d::Vector QUERY_OFFSET[NEARBY_COUNT] = {
	d2d::Vector( 0, -1),
	d2d::Vector(-1, -1),
	d2d::Vector(-1,  0),
	d2d::Vector(-1,  1),
	d2d::Vector( 0,  1),
	d2d::Vector( 1,  1),
	d2d::Vector( 1,  0),
	d2d::Vector( 1, -1),
};

ExtractOutlineRaw::ExtractOutlineRaw(const d2d::Image& image)
	: m_pixels(image.GetPixelData())
	, m_width(image.GetOriginWidth())
	, m_height(image.GetOriginHeight())
{
}

void ExtractOutlineRaw::CreateBorderPoints()
{
	m_border_points.clear();

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			bool is_border = IsPixelBorder(x, y);
			if (is_border) {
				m_border_points.push_back(d2d::Vector(x+0.5f, y+0.5f));
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
	d2d::ConvexHull::Implement(m_border_line, m_convex_hull);
}

void ExtractOutlineRaw::CreateBorderLine()
{
	m_border_line.clear();

	// find start, downmost and leftmost
	bool* flag = new bool[m_width * m_height];
	d2d::Vector first;
	first.setInvalid();
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			bool is_border = IsPixelBorder(x, y);
			flag[m_width*y+x] = is_border;
			if (is_border) {
				if (first.isValid()) {
					if (y < first.y || y == first.y && x < first.x) {
						first.set(x, y);
					}
				} else {
					first.set(x, y);
				}
			}
		}
	}

	d2d::Vector curr_pos = first;
	int curr_dir = -1;
	while (curr_pos.isValid()) 
	{
		// finish
		if (!m_border_line.empty() &&
			curr_pos == first) {
			m_border_axis_line.push_back(curr_pos + d2d::Vector(0.5f, 0.5f));
			break;
		}
		m_border_axis_line.push_back(curr_pos + d2d::Vector(0.5f, 0.5f));

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
			d2d::Vector nearby = curr_pos + QUERY_OFFSET[dir];
			// connect
			if (!IsPixelTransparente(flag, nearby.x, nearby.y)) {
				curr_dir = dir;
				curr_pos = nearby;
				break;
			} else {
				d2d::Vector new_pos = curr_pos;
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
	d2d::DouglasPeucker::implement(m_border_line, 1.5f, m_border_line_merged);
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
		if (IsPixelTransparente(x+NEARBY_OFFSET[i].x, y+NEARBY_OFFSET[i].y)) {
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

bool ExtractOutlineRaw::IsNearby(const d2d::Vector& p0, const d2d::Vector& p1) const
{
	return abs(p0.x - p1.x) <= 1 && abs(p0.y - p1.y) <= 1;
}

void ExtractOutlineRaw::MergeLine(const std::vector<d2d::Vector>& src, std::vector<d2d::Vector>& dst)
{
	if (src.size() < 3) {
		return;
	}
	dst.clear();
	dst.push_back(src[0]);
	dst.push_back(src[1]);
	for (int i = 2, n = src.size(); i < n; ++i)
	{
		const d2d::Vector& p = src[i];
		if (d2d::Math::IsTwoLineParallel(dst[dst.size()-1], dst[dst.size()-2], dst[dst.size()-1], p)) {
			dst.pop_back();
		}
		dst.push_back(p);
	}
}

}