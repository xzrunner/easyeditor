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

	std::vector<d2d::Vector> raw_border_merged;
//	MergeRawBorder(raw_border, raw_border_merged);
	d2d::DouglasPeucker::implement(raw_border, 1.5f, raw_border_merged);

	bool success = false;
	do {
		success = false;

		float area = d2d::Math::GetPolygonArea(fine_border);

		// remove one node
		d2d::Vector r_new0, r_new1;
		int r_idx = 0;
		float r_area_decrease = 0;
		for (int i = 0, n = fine_border.size(); i < n; ++i) {
			d2d::Vector _new0, _new1;
			float _area_decrease = 0;
			RemoveOneNode(fine_border, i, raw_border, _new0, _new1, _area_decrease);
			if (_area_decrease > r_area_decrease) {
				r_area_decrease = _area_decrease;
				r_new0 = _new0;
				r_new1 = _new1;
				r_idx = i;
			}
		}

		// add one node
		d2d::Vector a_new_start, a_new_end, a_new_node;
		int a_idx = 0;
		float a_area_decrease = 0;
 		for (int i = 0, n = fine_border.size(); i < n; ++i) {
 			d2d::Vector _new_start, _new_end, _new_node;
 			float _area_decrease = 0;
 			AddOneNode(fine_border, i, raw_border, raw_border_merged, r_area_decrease,
 				_new_start, _new_end, _new_node, _area_decrease);
 			if (_area_decrease > a_area_decrease) {
 				a_area_decrease = _area_decrease;
 				a_new_start = _new_start;
 				a_new_end = _new_end;
 				a_new_node = _new_node;
 				a_idx = i;
 			}
 		}

		// final
		if (r_area_decrease > a_area_decrease) {
			if (r_area_decrease / area > tolerance) {
				fine_border.erase(fine_border.begin()+r_idx);
				fine_border.insert(fine_border.begin()+r_idx, r_new0);
				fine_border.insert(fine_border.begin()+r_idx+1, r_new1);
				success = true;
			}
		} else {
			if (a_area_decrease / area > tolerance) {
				int sz = fine_border.size();
				bool cross;

				d2d::Vector new_start;
				cross = d2d::Math::GetTwoLineCross(a_new_node, a_new_start, 
					fine_border[(a_idx+sz-1)%sz], fine_border[a_idx], &new_start);
				if (cross) {
					fine_border[a_idx] = new_start;
				}

				d2d::Vector new_end;
				cross = d2d::Math::GetTwoLineCross(a_new_node, a_new_end,
					fine_border[(a_idx+2)%sz], fine_border[(a_idx+1)%sz], &new_end);
				if (cross) {
					fine_border[(a_idx+1)%sz] = new_end;
				}

				fine_border.insert(fine_border.begin()+a_idx+1, a_new_node);

				success = true;
			}
		}
	} while (success && fine_border.size() < 20);
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

void BoundaryExtraction::RemoveOneNode(const std::vector<d2d::Vector>& fine_border, 
									   int idx, 
									   const std::vector<d2d::Vector>& raw_border, 
									   d2d::Vector& new0, 
									   d2d::Vector& new1,
									   float& decrease) const
{
	decrease = 0;

	const d2d::Vector& curr = fine_border[idx];
	const d2d::Vector& prev = fine_border[(idx-1)%fine_border.size()];
	const d2d::Vector& next = fine_border[(idx+1)%fine_border.size()];
	float len_prev = d2d::Math::getDistance(curr, prev),
		  len_next = d2d::Math::getDistance(curr, next);
	float area_max = 0;
	float idx_e = 0;
	for (float i = 0; i < len_prev; i+=1) {
		d2d::Vector s = prev + (curr-prev)*(i/len_prev);
		for (float j = idx_e; j < len_next; j+=1) {
			d2d::Vector e = curr + (next-curr)*(j/len_next);
			if (IsSegmentLegal(raw_border, s, curr, e)) {
				float a = d2d::Math::GetTriangleArea(s, curr, e);
				if (a > decrease) {
					decrease = a;
					new0 = s;
					new1 = e;
				}
				idx_e = j;
			} else {
				break;
			}
		}
	}
}

void BoundaryExtraction::AddOneNode(const std::vector<d2d::Vector>& fine_border, 
									int idx, 
									const std::vector<d2d::Vector>& raw_border, 
									const std::vector<d2d::Vector>& raw_border_merged,
									float r_decrease,
									d2d::Vector& new_start,
									d2d::Vector& new_end, 
									d2d::Vector& new_node,
									float& decrease) const
{
	decrease = 0;
	const d2d::Vector& curr = fine_border[idx];
	const d2d::Vector& next = fine_border[(idx+1)%fine_border.size()];
	float len = d2d::Math::getDistance(curr, next);
	for (float idx_s = 0; idx_s < len; idx_s+=5) {
		d2d::Vector s = curr + (next-curr)*(idx_s/len);
		for (float idx_e = 0; idx_e < len-idx_s; idx_e+=5) {
			d2d::Vector e = next + (curr-next)*(idx_e/len);
			// todo new_node not need to traverse all raw_border
			for (int i = 0, n = raw_border.size(); i < n; ++i) {
				const d2d::Vector& _new = raw_border[i];
				float a = d2d::Math::GetTriangleArea(s, _new, e);
				if (a > decrease && a > r_decrease) {
//					if (IsSegmentLegal(raw_border, s, _new, e)) {

					d2d::Vector c = d2d::Math::getTriGravityCenter(s, e, _new);
					if (!d2d::Math::isPointInArea(c, fine_border)) {
						continue;
					}

					if (IsSegmentLegalNew(raw_border_merged, s, _new, e, _new)) {
						decrease = a;
						new_start = s;
						new_end = e;
						new_node = _new;
					}
				}
			}
		}
	}
}

bool BoundaryExtraction::IsSegmentLegal(const std::vector<d2d::Vector>& raw_border,
										const d2d::Vector& p0, const d2d::Vector& p1, 
										const d2d::Vector& p2) const
{
	if (d2d::Math::IsTwoLineParallel(p0, p1, p1, p2)) {
		return true;
	}

	static int last_idx = 0;
	for (int i = 0, n = raw_border.size(); i < n; i+=1) {
		if (d2d::Math::isPointInTriangle(raw_border[(last_idx+i)%n], p0 ,p1, p2)) {
			last_idx = i;
			return false;
		}
	}
	return true;
}

bool BoundaryExtraction::IsSegmentLegalNew(const std::vector<d2d::Vector>& raw_border_merged, 
										   const d2d::Vector& s0, const d2d::Vector& e0, 
										   const d2d::Vector& s1, const d2d::Vector& e1) const
{
	for (int i = 0, n = raw_border_merged.size(); i < n; ++i) {
		const d2d::Vector& s = raw_border_merged[i];
		const d2d::Vector& e = raw_border_merged[(i+1)%raw_border_merged.size()];
		if (d2d::Math::IsTwoLineIntersect(s, e, s0, e0) || d2d::Math::IsTwoLineIntersect(s, e, s1, e1)) {
			return false;
		}
	}
	return true;
}

void BoundaryExtraction::MergeRawBorder(const std::vector<d2d::Vector>& src, 
										std::vector<d2d::Vector>& dst) const
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