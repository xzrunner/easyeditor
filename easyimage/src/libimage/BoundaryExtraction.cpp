#include "BoundaryExtraction.h"

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

BoundaryExtraction::BoundaryExtraction(const d2d::Image& image)
	: m_pixels(image.getPixelData())
	, m_width(image.originWidth())
	, m_height(image.originHeight())
{
}

void BoundaryExtraction::GetRawBoundaryLine(std::vector<d2d::Vector>& border) const
{
	border.clear();

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
		if (!border.empty()) {
//			// no need
//			flag[int(pos.y*m_width+pos.x)] = false;
			// finish
			if (pos == first) {
				border.push_back(pos + d2d::Vector(0.5f, 0.5f));
				break;
			}
		}
		// insert pos
		border.push_back(pos + d2d::Vector(0.5f, 0.5f));

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

void BoundaryExtraction::GetRawBoundaryPoints(std::vector<d2d::Vector>& border) const
{
	border.clear();

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			bool is_border = IsPixelBorder(x, y);
			if (is_border) {
				border.push_back(d2d::Vector(x+0.5f, y+0.5f));
			}
		}
	}
}

void BoundaryExtraction::GetFineBoundaryLine(const std::vector<d2d::Vector>& raw_border, 
											 std::vector<d2d::Vector>& fine_border, 
											 float tolerance) const
{
	d2d::Rect r;
	for (int i = 0, n = raw_border.size(); i < n; ++i) {
		r.combine(raw_border[i]);
	}

	fine_border.push_back(d2d::Vector(r.xMin, r.yMin));
	fine_border.push_back(d2d::Vector(r.xMin, r.yMax));
	fine_border.push_back(d2d::Vector(r.xMax, r.yMax));
	fine_border.push_back(d2d::Vector(r.xMax, r.yMin));

	bool success = false;
	do {
		success = false;

//		float area = d2d::Math::isPointInArea();

		// remove one node
		for (int i = 0, n = fine_border.size(); i < n; ++i) {

		}

		// add one node

	} while (success);
}

bool BoundaryExtraction::IsPixelBorder(int x, int y) const
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

bool BoundaryExtraction::IsPixelTransparente(int x, int y) const
{
	if (x < 0 || x >= m_width ||
		y < 0 || y >= m_height) {
		return true;
	}
	
	byte alpha = m_pixels[(y*m_width+x)*4+3];
	return alpha == 0;
}

bool BoundaryExtraction::IsPixelTransparente(bool* flag, int x, int y) const
{
	if (x < 0 || x >= m_width ||
		y < 0 || y >= m_height) {
		return true;
	}

	return !flag[y*m_width+x];
}

bool BoundaryExtraction::IsNearby(const d2d::Vector& p0, const d2d::Vector& p1) const
{
	return abs(p0.x - p1.x) <= 1 && abs(p0.y - p1.y) <= 1;
}

}