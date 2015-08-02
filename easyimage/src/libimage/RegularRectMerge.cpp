#include "RegularRectMerge.h"
#include "PixelUncoveredLUT.h"
#include "RegularRectCondense.h"

namespace eimage
{

RegularRectMerge::RegularRectMerge(const std::vector<Rect>& rects, int width, 
								   int height, bool* ori_pixels)
	: m_width(width)
	, m_height(height)
{
	m_uncovered_lut = new PixelUncoveredLUT(width, height);
	m_condense = new RegularRectCondense(rects, width, height, ori_pixels);
}

RegularRectMerge::~RegularRectMerge()
{
	delete m_condense;
	delete m_uncovered_lut;
}

void RegularRectMerge::Merge()
{
	static const int COUNT_MIN = 6;
	static const int AREA_MIN = 16 * 16;
	static const int ENLARGE_MAX = 16 * 16 * 2;

	int debug_count = 0;

	m_condense->Condense(Rect(0, 0, m_width, m_height));

	while (true) 
	{
		std::vector<Rect> rects;
		m_condense->GetSortedRects(rects);

		const Rect& r0 = rects[0];
		assert(r0.w > 0 && r0.h > 0);
		if (r0.w * r0.h > AREA_MIN && rects.size() < COUNT_MIN) {
			break;
		}

		m_uncovered_lut->LoadRects(rects);

		int cost_min = INT_MAX;
		int r_min = -1;
		for (int i = 1, n = rects.size(); i < n; ++i) {
			const Rect& r1 = rects[i];

			int left = std::min(r0.x, r1.x),
				right = std::max(r0.x + r0.w, r1.x + r1.w);
			int down = std::min(r0.y, r1.y),
				up = std::max(r0.y + r0.h, r1.y + r1.h);
			Rect mr(left, down, right - left, up - down);

			int cost = ComputeCost(mr, rects);
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

		assert(r_min != -1);

		const Rect& r1 = rects[r_min];

		int left = std::min(r0.x, r1.x),
			right = std::max(r0.x + r0.w, r1.x + r1.w);
		int down = std::min(r0.y, r1.y),
			up = std::max(r0.y + r0.h, r1.y + r1.h);
		Rect mr(left, down, right - left, up - down);

// 		if (debug_count == 11) {
// 			int zz = 0;
// 		}

		m_condense->Remove(rects[0]);
		m_condense->Remove(rects[r_min]);
		m_condense->Insert(mr);
		m_condense->Condense(mr);

		++debug_count;
// 		if (debug_count == 12) {
// 			break;
// 		}
	}
}

void RegularRectMerge::GetResult(std::vector<Rect>& result) const
{
	m_condense->GetSortedRects(result);
}

int RegularRectMerge::ComputeCost(const Rect& r, const std::vector<Rect>& rects) const
{
	for (int i = 0, n = rects.size(); i < n; ++i) 
	{
		const Rect& b = rects[i];

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

	int cost = m_uncovered_lut->GetUncoveredArea(r.x, r.y, r.w, r.h);
	return cost;
}

}