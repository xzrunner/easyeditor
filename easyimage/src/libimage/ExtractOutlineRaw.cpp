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
//static const int QUERTY_COUNT = 4;
//static const d2d::Vector QUERTY_OFFSET[NEARBY_COUNT] = {
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
static const int QUERTY_COUNT = 8;
static const d2d::Vector QUERTY_OFFSET[NEARBY_COUNT] = {
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
	: m_pixels(image.getPixelData())
	, m_width(image.originWidth())
	, m_height(image.originHeight())
{
}

void ExtractOutlineRaw::Trigger()
{
	CreateBorderLine();
	CreateBorderPoints();
	MergeBorderLine();
}

void ExtractOutlineRaw::CreateBorderLine()
{
	m_border_line.clear();

	// find start
	bool* flag = new bool[m_width * m_height];
	d2d::Vector first;
	first.setInvalid();
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			bool is_border = IsPixelBorder(x, y);
			flag[m_width*y+x] = is_border;
			if (is_border && !first.isValid()) {
				first.set(x, y);
			}
		}
	}

	std::queue<d2d::Vector> buf;
	if (first.isValid()) {
		buf.push(first);
	}
	int last_dir = -1;
	while (!buf.empty()) 
	{
		d2d::Vector pos = buf.front(); buf.pop();
		// the first will not set flag, for final connect
		if (!m_border_line.empty()) {
//			// no need
//			flag[int(pos.y*m_width+pos.x)] = false;
			// finish
			if (pos == first) {
				m_border_line.push_back(pos + d2d::Vector(0.5f, 0.5f));
				break;
			}
		}
		// insert pos
		m_border_line.push_back(pos + d2d::Vector(0.5f, 0.5f));

		int curr;
		// the first, start from [0]
		if (last_dir == -1) {
			curr = 0;
			first = pos;
		// curr dir is next of inverse last dir
		} else {
			curr = (last_dir + (QUERTY_COUNT >> 1)) + 1;
		}
		// search in certain order
		for (int i = 0; i < QUERTY_COUNT; ++i) {
			int dir = (i+curr)%QUERTY_COUNT;
			d2d::Vector nearby = pos + QUERTY_OFFSET[dir];
			// connect
			if (!IsPixelTransparente(flag, nearby.x, nearby.y)) {
				last_dir = dir;
				buf.push(nearby);
				break;
			}
		}
	}

	delete[] flag;
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

void ExtractOutlineRaw::MergeBorderLine()
{
	m_border_merged.clear();
	//	MergeRawBorder(border, border_merged);
	d2d::DouglasPeucker::implement(m_border_line, 1.5f, m_border_merged);
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
	
	byte alpha = m_pixels[(y*m_width+x)*4+3];
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