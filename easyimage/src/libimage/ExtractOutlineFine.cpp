#include "ExtractOutlineFine.h"

namespace eimage
{

static const int STEPS_COUNT = 3;
static const float STEPS[STEPS_COUNT] = {1/8.0f, 1/16.0f, 1/32.0f};

ExtractOutlineFine::ExtractOutlineFine(const std::vector<d2d::Vector>& raw_border, 
									   const std::vector<d2d::Vector>& raw_border_merged)
	: m_raw_border(raw_border)
	, m_raw_border_merged(raw_border_merged)
{
}

void ExtractOutlineFine::Trigger(float tolerance)
{
	OutlineByAddNode(tolerance, INT_MAX, true);	
}

void ExtractOutlineFine::CreateOutline(float tolerance, int max_count)
{
	OutlineByAddNode(tolerance, max_count, false);
}

void ExtractOutlineFine::ReduceOutlineCount(float tolerance)
{
	ReduceNode(tolerance);
	ReduceEdge(tolerance);
}

void ExtractOutlineFine::OutlineByAddNode(float tolerance, int max_count,
										  bool reduce_count)
{
	m_fine_border.clear();

	d2d::Rect r;
	for (int i = 0, n = m_raw_border.size(); i < n; ++i) {
		r.combine(m_raw_border[i]);
	}

	m_fine_border.push_back(d2d::Vector(r.xMin, r.yMin));
	m_fine_border.push_back(d2d::Vector(r.xMin, r.yMax));
	m_fine_border.push_back(d2d::Vector(r.xMax, r.yMax));
	m_fine_border.push_back(d2d::Vector(r.xMax, r.yMin));

	std::vector<d2d::Vector> last_fine_border = m_fine_border;

	bool success = false;
	do {
		success = false;

		int sz = m_fine_border.size();

		float area = d2d::Math::GetPolygonArea(m_fine_border);

		// remove one node
		d2d::Vector r_new0, r_new1;
		int r_idx = 0;
		float r_area_decrease = 0;
		for (int i = 0; i < sz; ++i) {
			d2d::Vector _new0, _new1;
			float _area_decrease = 0;
			RemoveOneNode(i, _new0, _new1, _area_decrease);
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
		for (int i = 0; i < sz; ++i) {
			d2d::Vector _new_start, _new_end, _new_node;
			float _area_decrease = 0;
			AddOneNode(i, r_area_decrease, _new_start, _new_end, 
				_new_node, _area_decrease);
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
				m_fine_border.erase(m_fine_border.begin()+r_idx);
				m_fine_border.insert(m_fine_border.begin()+r_idx, r_new0);
				m_fine_border.insert(m_fine_border.begin()+r_idx+1, r_new1);
				success = true;
			}
		} else {
			if (a_area_decrease / area > tolerance) {
				bool cross;

				d2d::Vector new_start;
				cross = d2d::Math::GetTwoLineCross(a_new_node, a_new_start, 
					m_fine_border[(a_idx+sz-1)%sz], m_fine_border[a_idx], &new_start);
				if (cross) {
					m_fine_border[a_idx] = new_start;
				}

				d2d::Vector new_end;
				cross = d2d::Math::GetTwoLineCross(a_new_node, a_new_end,
					m_fine_border[(a_idx+2)%sz], m_fine_border[(a_idx+1)%sz], &new_end);
				if (cross) {
					m_fine_border[(a_idx+1)%sz] = new_end;
				}

				m_fine_border.insert(m_fine_border.begin()+a_idx+1, a_new_node);

				success = true;
			}
		}

		// reduce count
		if (reduce_count) {
			ReduceOutlineCount(tolerance);
		}

		// test stop
		if (m_fine_border.size() == last_fine_border.size()) {
			bool stop = true;
			for (int i = 0, n = m_fine_border.size(); i < n; ++i) {
				if (m_fine_border[i] != last_fine_border[i]) {
					stop = false;
					break;
				}
			}
			if (stop) {
				break;
			}
		}
		last_fine_border = m_fine_border;

	} while (success && m_fine_border.size() < max_count);
}

void ExtractOutlineFine::RemoveOneNode(int idx, d2d::Vector& new0, d2d::Vector& new1, float& decrease) const
{
	decrease = 0;

	int sz = m_fine_border.size();
	const d2d::Vector& curr = m_fine_border[idx];
	const d2d::Vector& prev = m_fine_border[(idx+sz-1)%sz];
	const d2d::Vector& next = m_fine_border[(idx+1)%sz];
	d2d::Vector s0 = curr - prev, s1 = next - curr;
	if (f2Cross(s0, s1) >= 0) {
		return;
	}

	float len_prev = d2d::Math::getDistance(curr, prev),
		len_next = d2d::Math::getDistance(curr, next);
	float area_max = 0;
	float idx_e = 0;
	for (float i = 0; i < len_prev; i+=1) {
		d2d::Vector s = prev + (curr-prev)*(i/len_prev);
		for (float j = idx_e; j < len_next; j+=1) {
			d2d::Vector e = curr + (next-curr)*(j/len_next);
			if (IsCutTriLegal(s, curr, e)) {
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

// todo 太小边的探索尽快终止掉
void ExtractOutlineFine::AddOneNode(int idx, float r_decrease, d2d::Vector& new_start,
									d2d::Vector& new_end, d2d::Vector& new_node, float& decrease) const
{
	decrease = 0;
	int sz = m_fine_border.size();
	const d2d::Vector& curr = m_fine_border[idx];
	const d2d::Vector& next = m_fine_border[(idx+1)%sz];
	float best_score = 0;
	d2d::Vector best_node;
	// init region
	float best_start_scale, best_end_scale;
	assert(STEPS_COUNT > 0);
	float step = STEPS[0];
	for (float start_scale = 0; start_scale < 1; start_scale += step) {
		d2d::Vector start_pos = curr + (next-curr)*start_scale;
		for (float end_scale = 1; end_scale > start_scale; end_scale -= step) {
			d2d::Vector end_pos = curr + (next-curr)*end_scale;
			d2d::Vector mid_pos;
			float score;
			MidPosExplore(start_pos, end_pos, mid_pos, score);
			if (score > best_score) {
				best_score = score;
				best_node = mid_pos;
				best_start_scale = start_scale;
				best_end_scale = end_scale;
			}
		}
	}
	// refine region
	StartPosExplore(curr, next, best_start_scale, best_end_scale, best_node, best_score);
	
	new_start = curr + (next-curr)*best_start_scale;
	new_end = curr + (next-curr)*best_end_scale;
	new_node = best_node;
	decrease = best_score;
}

bool ExtractOutlineFine::IsCutTriLegal(const d2d::Vector& p0, const d2d::Vector& p1, 
										const d2d::Vector& p2) const
{
	if (d2d::Math::IsTwoLineParallel(p0, p1, p1, p2)) {
		return true;
	}

	static int last_idx = 0;
	for (int i = 0, n = m_raw_border.size(); i < n; i+=1) {
		if (d2d::Math::isPointInTriangle(m_raw_border[(last_idx+i)%n], p0 ,p1, p2)) {
			last_idx = i;
			return false;
		}
	}
	return true;
}

bool ExtractOutlineFine::IsAddTriLeagal(const d2d::Vector& start, const d2d::Vector& end, 
										   const d2d::Vector& center) const
{
	for (int i = 0, n = m_raw_border_merged.size(); i < n; ++i) {
		if (d2d::Math::isPointInTriangle(m_raw_border_merged[i], start, end, center)) {
			return false;
		}
	}
	if (d2d::Math::IsSegmentIntersectPolyline(start, center, m_raw_border_merged) ||
		d2d::Math::IsSegmentIntersectPolyline(end, center, m_raw_border_merged)) {
		return false;
	}
	if (d2d::Math::IsSegmentIntersectPolyline(start, center, m_fine_border) ||
		d2d::Math::IsSegmentIntersectPolyline(end, center, m_fine_border)) {
		return false;
	}
	return true;
}

// 已经是算出来的
void ExtractOutlineFine::StartPosExplore(const d2d::Vector& p0, const d2d::Vector& p1, 
										 float& start_scale, float& end_scale, 
										 d2d::Vector& mid, float& score) const
{
	for (int s = 1; s < STEPS_COUNT; ++s)
	{
		float step = STEPS[s];

		// sub
		float sub_start_scale = start_scale - step;
		float sub_end_scale = end_scale;
		float sub_score = 0;
		d2d::Vector sub_mid;
		if (sub_start_scale <= 1 && sub_start_scale < end_scale) {
			EndPosExplore(step, p0, p1, sub_start_scale, sub_end_scale, sub_mid, sub_score);
		}
		// add
		float add_start_scale = start_scale + step;
		float add_end_scale = end_scale;
		float add_score = 0;
		d2d::Vector add_mid;
		if (add_start_scale <= 1 && add_start_scale < end_scale) {
			EndPosExplore(step, p0, p1, add_start_scale, add_end_scale, add_mid, add_score);
		}
		// decide direction
		if (sub_score <= score && add_score <= score) {
			return;
		} else if (sub_score > score) {
			step = -STEPS[s];
			start_scale = sub_start_scale;
			end_scale = sub_end_scale;
			score = sub_score;
			mid = sub_mid;
		} else {
			step = STEPS[s];
			start_scale = add_start_scale;
			end_scale = add_end_scale;
			score = add_score;
			mid = add_mid;
		}
		// go on
		float curr_start_scale = end_scale + step;
		while (curr_start_scale <= 1 && curr_start_scale < end_scale)
		{
			float old_score = score;
			EndPosExplore(step, p0, p1, curr_start_scale, end_scale, mid, score);
			if (score > old_score) {
				start_scale = curr_start_scale;
				curr_start_scale += step;
			} else {
				break;
			}
		}
	}
}

// 参数的score没算出
void ExtractOutlineFine::EndPosExplore(float step, const d2d::Vector& p0, const d2d::Vector& p1, float start_scale, 
									   float& end_scale, d2d::Vector& mid, float& score) const
{
 	d2d::Vector start_pos = p0 + (p1-p0)*start_scale,
 		end_pos = p0 + (p1-p0)*end_scale;
	float curr_score;
	MidPosExplore(start_pos, end_pos, mid, curr_score);
	if (curr_score <= score) {
		return;
	} else {
		score = curr_score;
	}

	float curr_step = step;
	// sub
	float sub_end_scale = end_scale - curr_step;
	float sub_score = 0;
	d2d::Vector sub_mid;
	if (sub_end_scale <= 1 && sub_end_scale > start_scale) {
		d2d::Vector e = p0 + (p1-p0)*sub_end_scale;
		MidPosExplore(start_pos, e, sub_mid, sub_score);
	}
	// add
	float add_end_scale = end_scale + curr_step;
	float add_score = 0;
	d2d::Vector add_mid;
	if (add_end_scale <= 1 && add_end_scale > start_scale) {
		d2d::Vector e = p0 + (p1-p0)*add_end_scale;
		MidPosExplore(start_pos, e, add_mid, add_score);
	}
	// decide direction
	if (sub_score <= score && add_score <= score) {
		return;
	} else if (sub_score > score) {
		curr_step = -step;
		end_scale = sub_end_scale;
		score = sub_score;
		mid = sub_mid;
	} else {
		curr_step = step;
		end_scale = add_end_scale;
		score = add_score;
		mid = add_mid;
	}
	// go on
	float curr_end_scale = end_scale + curr_step;
	while (curr_end_scale <= 1 && curr_end_scale > start_scale)
	{
		float curr_score;
		d2d::Vector e = p0 + (p1-p0)*curr_end_scale;
		MidPosExplore(start_pos, e, mid, curr_score);
		if (curr_score > score) {
			score = curr_score;
			end_scale = curr_end_scale;
			curr_end_scale += curr_step;
		} else {
			break;
		}
	}
}

void ExtractOutlineFine::MidPosExplore(const d2d::Vector& start, const d2d::Vector& end, 
									   d2d::Vector& mid, float& score) const
{
	// order
	// 3 4 5
	// 2   6
	// 1 0 7
	static const int DIR_COUNT = 8;
	static const d2d::Vector DIRS[DIR_COUNT] = {
		d2d::Vector( 0, -1),
		d2d::Vector(-1, -1),
		d2d::Vector(-1,  0),
		d2d::Vector(-1,  1),
		d2d::Vector( 0,  1),
		d2d::Vector( 1,  1),
		d2d::Vector( 1,  0),
		d2d::Vector( 1, -1),
	};

	static const int STEPS_COUNT = 6;
	static const float STEPS[STEPS_COUNT] = {128, 64, 32, 16, 8, 4};

	mid = (start + end) * 0.5f;
	score = 0;
	for (int i = 0; i < STEPS_COUNT; ++i) {
		float step = STEPS[i];

		while (true)
		{
			float old_score = score;
			for (int j = 0; j < DIR_COUNT; ++j) {
				d2d::Vector offset = DIRS[j] * step;
				d2d::Vector curr_mid = mid + offset;
				if (d2d::Math::isPointInArea(curr_mid, m_fine_border)) {
					float area = d2d::Math::GetTriangleArea(start, curr_mid, end);
					if (area > score && IsAddTriLeagal(start, end, curr_mid)) {
						score = area;
						mid = curr_mid;
					}
				}
			}
			if (score <= old_score) {
				break;
			}
		}
	}
}

void ExtractOutlineFine::ReduceNode(float tolerance)
{
	if (m_fine_border.size() <= 3) {
		return;
	}

	float area_limit = d2d::Math::GetPolygonArea(m_fine_border) * tolerance;

	bool success = false;
	do {
		success = false;
		int sz = m_fine_border.size();
		for (int i = 0; i < sz; ++i)
		{
			const d2d::Vector& curr = m_fine_border[i];
			const d2d::Vector& prev = m_fine_border[(i+sz-1)%sz];
			const d2d::Vector& next = m_fine_border[(i+1)%sz];
			d2d::Vector s0 = curr - prev, s1 = next - curr;
			if (f2Cross(s0, s1) >= 0) {
				float a = d2d::Math::GetTriangleArea(curr, prev, next);
				if (a < area_limit && 
					!d2d::Math::IsSegmentIntersectPolyline(prev, next, m_fine_border)) 
				{
					m_fine_border.erase(m_fine_border.begin() + i);
					success = true;
					break;
				}
			} else {
				if (IsCutTriLegal(curr, prev, next)) {
					m_fine_border.erase(m_fine_border.begin() + i);
					success = true;
					break;
				}
			}
		}
	} while (success && m_fine_border.size() > 3);
}

void ExtractOutlineFine::ReduceEdge(float tolerance)
{
	if (m_fine_border.size() <= 3) {
		return;
	}

	float area_limit = d2d::Math::GetPolygonArea(m_fine_border) * tolerance;
	bool success = false;
	do {
		success = false;
		int sz = m_fine_border.size();
		for (int i = 0; i < sz; ++i)
		{
			const d2d::Vector& start = m_fine_border[i];
			const d2d::Vector& start_prev = m_fine_border[(i+sz-1)%sz];
			const d2d::Vector& end = m_fine_border[(i+1)%sz];
			const d2d::Vector& end_next = m_fine_border[(i+2)%sz];
			
			d2d::Vector intersect;
			bool cross = d2d::Math::GetTwoLineCross(start_prev, start, end, end_next, &intersect);
			if (!cross) {
				continue;
			}

			bool inside_s = d2d::Math::isBetween(start_prev.x, start.x, intersect.x) 
				&& d2d::Math::isBetween(start_prev.y, start.y, intersect.y);
			bool inside_e = d2d::Math::isBetween(end.x, end_next.x, intersect.x) 
				&& d2d::Math::isBetween(end.y, end_next.y, intersect.y);
			if (!inside_s && !inside_e) 
			{
				float a = d2d::Math::GetTriangleArea(intersect, start, end);
				if (a < area_limit && 
					!d2d::Math::IsSegmentIntersectPolyline(intersect, start, m_fine_border) &&
					!d2d::Math::IsSegmentIntersectPolyline(intersect, end, m_fine_border)) 
				{
					m_fine_border[i] = intersect;
					m_fine_border.erase(m_fine_border.begin() + (i+1)%sz);
					success = true;
					break;
				}
			} 
			else 
			{
				if (inside_s && inside_e) {
					return;
				}

				assert(!inside_s || !inside_e);
				if (IsCutTriLegal(intersect, start, end)) {
					if (inside_s) {
						m_fine_border[i] = intersect;
						m_fine_border.erase(m_fine_border.begin() + (i+1)%sz);
					} else {
						m_fine_border[(i+1)%sz] = intersect;
						m_fine_border.erase(m_fine_border.begin() + i);
					}
					success = true;
					break;
				}
			}
		}
	} while (success && m_fine_border.size() > 3);
}

}