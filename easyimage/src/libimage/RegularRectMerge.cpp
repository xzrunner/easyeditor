#include "RegularRectMerge.h"
#include "PixelCoveredLUT.h"
#include "RectPostProcessor.h"

namespace eimage
{

RegularRectMerge::RegularRectMerge(int width, int height, 
								   const std::vector<Rect>& rects,
								   RectPostProcessor& proc)
	: m_proc(proc)
	, m_rects(rects)
{
	m_covered_area = new PixelCoveredLUT(width, height, rects);
}

RegularRectMerge::~RegularRectMerge()
{
	delete m_covered_area;
}

void RegularRectMerge::Merge()
{
	static const int COUNT_MIN = 6;
	static const int AREA_MIN = 16 * 16;
	static const int ENLARGE_MAX = 16 * 16 * 4;

	int debug_count = 0;

	bool need_sort = true;
	while (true) {
		if (m_rects.size() < COUNT_MIN) {
			break;
		}

		if (need_sort) {
			std::sort(m_rects.begin(), m_rects.end(), RectCmp());
		}

		const Rect& r0 = m_rects[0];
		if (r0.w * r0.h > AREA_MIN) {
			break;
		}

		int cost_min = INT_MAX;
		int r_min = -1;
		for (int i = 1, n = m_rects.size(); i < n; ++i) {
			const Rect& r1 = m_rects[i];

			int left = std::min(r0.x, r1.x),
				right = std::max(r0.x + r0.w, r1.x + r1.w);
			int down = std::min(r0.y, r1.y),
				up = std::max(r0.y + r0.h, r1.y + r1.h);
			Rect mr(left, down, right - left, up - down);

			int cost = ComputeCost(mr);
			if (cost == 0) {
				cost_min = 0;
				r_min = i;
				break;
			} else if (cost < cost_min) {
				cost_min = cost;
				r_min = i;
			}
		}

		if (cost_min > ENLARGE_MAX) {
			break;
		}

		if (r_min != -1) {
			const Rect& r1 = m_rects[r_min];

			int left = std::min(r0.x, r1.x),
				right = std::max(r0.x + r0.w, r1.x + r1.w);
			int down = std::min(r0.y, r1.y),
				up = std::max(r0.y + r0.h, r1.y + r1.h);
			Rect mr(left, down, right - left, up - down);

			if (debug_count == 18) {
				int zz = 0;
			}

			// LUT
			m_covered_area->Remove(m_rects[0]);
			m_covered_area->Remove(m_rects[r_min]);
			m_covered_area->Insert(mr);

			// proc
			m_proc.RemoveItem(m_rects[0]);
			m_proc.RemoveItem(m_rects[r_min]);
			m_proc.InsertItem(mr);
			m_proc.Condense(mr, m_covered_area);

			// self
// 			m_rects.erase(m_rects.begin() + r_min);
// 			m_rects.erase(m_rects.begin());
// 			m_rects.push_back(mr);
			m_proc.LoadResult(m_rects);
			need_sort = true;

			++debug_count;
			if (debug_count == 20) {
				break;
			}

		} else {
			need_sort = false;
		}
	}
}

int RegularRectMerge::ComputeCost(const Rect& r) const
{
	for (int i = 0, n = m_rects.size(); i < n; ++i) {
		const Rect& b = m_rects[i];

		if (r.x > b.x && (r.x + r.w < b.x + b.w) &&
			r.y < b.y && (r.y + r.h > b.y + b.h)) {
			return INT_MAX;
		} 
		if (b.x > r.x && (b.x + b.w < r.x + r.w) &&
			b.y < r.y && (b.y + b.h > r.y + r.h)) {
			return INT_MAX;
		}

		if (r.x >= b.x + 1 && (r.x < b.x + b.w) && r.y >= b.y + 1 && (r.y < b.y + b.h) &&
			(r.x + r.w > b.x + b.w) && (r.y + r.h > b.y + b.h)) {
			return INT_MAX;
		} 
		if (r.x >= b.x + 1 && (r.x < b.x + b.w) && (r.y + r.h - 1 >= b.y) && (r.y + r.h - 1 < b.y + b.h) &&
			(r.x + r.w > b.x + b.w) && (r.y < b.y)) {
			return INT_MAX;
		}
		if ((r.x + r.w - 1 >= b.x) && (r.x + r.w - 1 < b.x + b.w) && (r.y + r.h - 1 >= b.y) && (r.y + r.h - 1 < b.y + b.h) &&
			r.x < b.x && r.y < b.y) {
			return INT_MAX;
		}
		if ((r.x + r.w - 1 >= b.x) && (r.x + r.w - 1 < b.x + b.w) && (r.y >= b.y + 1) && (r.y < b.y + b.h) &&
			r.x < b.x && (r.y + r.h > b.y + b.h)) {
			return INT_MAX;
		}
	}

	int cost = r.w * r.h - m_covered_area->GetCoveredArea(r.x, r.y, r.w, r.h);
	return cost;
}

}