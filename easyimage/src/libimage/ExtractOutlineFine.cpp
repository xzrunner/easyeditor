#include "ExtractOutlineFine.h"

#include <ee/MinBoundingBox.h>
#include <ee/Math2D.h>

#include <SM_Test.h>
#include <SM_Calc.h>

#include <assert.h>

namespace eimage
{

static const int STEPS_COUNT = 3;
static const float STEPS[STEPS_COUNT] = {1/8.0f, 1/16.0f, 1/32.0f};

ExtractOutlineFine::ExtractOutlineFine(const std::vector<sm::vec2>& raw_border, 
									   const std::vector<sm::vec2>& raw_border_merged)
	: m_raw_border(raw_border)
	, m_raw_border_merged(raw_border_merged)
{
	InitRotateLUT();
}

void ExtractOutlineFine::Trigger(float area_tol, float perimeter_tol)
{
	OutlineByAddNode(area_tol, perimeter_tol, INT_MAX, true);	
}

void ExtractOutlineFine::Trigger(float area_tol, float perimeter_tol, int max_step)
{
	OutlineByAddNode(area_tol, perimeter_tol, max_step, true);		
}

void ExtractOutlineFine::CreateOutline(float area_tol, float perimeter_tol, int max_step)
{
	OutlineByAddNode(area_tol, perimeter_tol, max_step, false);
}

void ExtractOutlineFine::ReduceOutlineCount(float area_tol, float perimeter_tol)
{
	ReduceEdge(area_tol, perimeter_tol);
}

void ExtractOutlineFine::OutlineByAddNode(float area_tol, float perimeter_tol, 
										  int max_step, bool reduce_count)
{
	m_fine_border.clear();

	sm::vec2 bound[4];
	ee::MinBoundingBox::Do(m_raw_border, bound);
	m_fine_border.assign(bound, bound+4);

	std::vector<sm::vec2> last_fine_border = m_fine_border;

	int count = 0;
	bool success = false;
	do {
		success = false;

		float area = ee::Math2D::GetPolygonArea(m_fine_border);

		// remove one node
		sm::vec2 r_new0, r_new1;
		int r_idx = 0;
		float r_area_decrease = 0;
		for (int i = 0, n = m_fine_border.size(); i < n; ++i) {
			sm::vec2 _new0, _new1;
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
		sm::vec2 a_new_start, a_new_end, a_new_node;
		int a_idx = 0;
		float a_area_decrease = 0;
		for (int i = 0, n = m_fine_border.size(); i < n; ++i) {
			sm::vec2 _new_start, _new_end, _new_node;
			float _area_decrease = 0;
			AddOneNode(i, _new_start, _new_end, _new_node, _area_decrease);
			if (_area_decrease > a_area_decrease) {
				a_area_decrease = _area_decrease;
				a_new_start = _new_start;
				a_new_end = _new_end;
				a_new_node = _new_node;
				a_idx = i;
			}
		}

		// final
		static const float REMOVE_WEIGHT = 1.15f;
		if (r_area_decrease * REMOVE_WEIGHT > a_area_decrease) {
			if (r_area_decrease / area > area_tol) {
				m_fine_border.erase(m_fine_border.begin()+r_idx);
				m_fine_border.insert(m_fine_border.begin()+r_idx, r_new0);
				m_fine_border.insert(m_fine_border.begin()+((r_idx+1)%m_fine_border.size()), r_new1);
				success = true;
			}
		} else {
			if (a_area_decrease / area > area_tol) {
				const sm::vec2& start = m_fine_border[a_idx];
				const sm::vec2& start_prev = m_fine_border[(a_idx+m_fine_border.size()-1)%m_fine_border.size()];
				sm::vec2 cross_start;
				int new_node_pos;
				sm::intersect_line_line(a_new_node, a_new_start, start_prev, start, &cross_start);
				if (sm::dis_square_pos_to_pos(cross_start, a_new_start) < sm::dis_square_pos_to_pos(cross_start, a_new_node) &&
					sm::dis_square_pos_to_pos(cross_start, a_new_node) >= sm::dis_square_pos_to_pos(a_new_start, a_new_node)) {
					new_node_pos = (a_idx+1)%m_fine_border.size();
					m_fine_border[a_idx] = cross_start;
				} else {					
					std::vector<sm::vec2>::iterator itr = 
						m_fine_border.insert(m_fine_border.begin()+((a_idx+1)%m_fine_border.size()), a_new_start);
					++itr;
					if (itr == m_fine_border.end()) {
						itr = m_fine_border.begin();
					}
					new_node_pos = std::distance(m_fine_border.begin(), itr);
				}
				m_fine_border.insert(m_fine_border.begin()+new_node_pos, a_new_node);

				const sm::vec2& end = m_fine_border[(new_node_pos+1)%m_fine_border.size()];
				const sm::vec2& end_next = m_fine_border[(new_node_pos+2)%m_fine_border.size()];
				sm::vec2 cross_end;
				sm::intersect_line_line(a_new_node, a_new_end, end_next, end, &cross_end);
				if (sm::dis_square_pos_to_pos(cross_end, a_new_end) < sm::dis_square_pos_to_pos(cross_end, a_new_node) &&
					sm::dis_square_pos_to_pos(cross_end, a_new_node) >= sm::dis_square_pos_to_pos(a_new_end, a_new_node)) {
					m_fine_border[(new_node_pos+1)%m_fine_border.size()] = cross_end;
				} else {
					m_fine_border.insert(m_fine_border.begin()+((new_node_pos+1)%m_fine_border.size()), a_new_end);
				}

				success = true;
			}
		}

		// reduce count
		if (reduce_count) {
			ReduceOutlineCount(area_tol, perimeter_tol);
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

		assert(ee::Math2D::IsPolygonInPolygon(m_raw_border_merged, m_fine_border));

// 		// test legal
// 		if (!IsOutlineLegal()) {
// 			m_fine_border = last_fine_border;
// 			std::cout << "!!!!!!!!!!!!!!!!!!!!! Error \n";
// 			break;
// 		}

		last_fine_border = m_fine_border;
//	} while (++count < max_step && success);
	} while (++count < max_step);
}

void ExtractOutlineFine::RemoveOneNode(int idx, sm::vec2& new0, sm::vec2& new1, float& decrease) const
{
	decrease = 0;

	const sm::vec2& curr = m_fine_border[idx];
	const sm::vec2& prev = m_fine_border[(idx+m_fine_border.size()-1)%m_fine_border.size()];
	const sm::vec2& next = m_fine_border[(idx+1)%m_fine_border.size()];
	sm::vec2 s0 = curr - prev, s1 = next - curr;
	if (s0.Cross(s1) >= 0) {
		return;
	}

	float len_prev = sm::dis_pos_to_pos(curr, prev),
		len_next = sm::dis_pos_to_pos(curr, next);
	float area_max = 0;
	float idx_e = 0;
	for (float i = 0; i < len_prev; i+=1) {
		sm::vec2 s = prev + (curr-prev)*(i/len_prev);
		for (float j = idx_e; j < len_next; j+=1) {
			sm::vec2 e = curr + (next-curr)*(j/len_next);
			if (IsCutTriLegal(curr, s, e)) {
				float a = ee::Math2D::GetTriangleArea(s, curr, e);
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
void ExtractOutlineFine::AddOneNode(int idx, sm::vec2& new_start, sm::vec2& new_end, 
									sm::vec2& new_node, float& decrease) const
{
	decrease = 0;
	const sm::vec2& curr = m_fine_border[idx];
	const sm::vec2& next = m_fine_border[(idx+1)%m_fine_border.size()];
	float best_score = 0;
	sm::vec2 best_node;
	// init region
	float best_start_scale, best_end_scale;
	assert(STEPS_COUNT > 0);
	float step = STEPS[0];
	for (float start_scale = 0; start_scale < 1; start_scale += step) {
		sm::vec2 start_pos = curr + (next-curr)*start_scale;
		for (float end_scale = 1; end_scale > start_scale; end_scale -= step) {
			sm::vec2 end_pos = curr + (next-curr)*end_scale;
			sm::vec2 mid_pos;
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

bool ExtractOutlineFine::IsCutTriLegal(const sm::vec2& center, 
									   const sm::vec2& p0, 
									   const sm::vec2& p1) const
{
	if (ee::Math2D::IsSegmentIntersectPolyline(p0, p1, m_fine_border)) {
		return false;
	}

	sm::vec2 tri_v[3] = {center, p0, p1};
	std::vector<sm::vec2> tri;
	tri.assign(tri_v, tri_v+3);
	if (ee::Math2D::IsPolygonIntersectPolygon(m_raw_border_merged, tri)) {
		return false;
	}

	return true;
}

bool ExtractOutlineFine::IsAddTriLeagal(const sm::vec2& p0, const sm::vec2& p1, const sm::vec2& p2) const
{
	sm::vec2 tri_v[3] = {p0, p1, p2};
	std::vector<sm::vec2> tri;
	tri.assign(tri_v, tri_v+3);
	return !ee::Math2D::IsPolygonIntersectPolygon(m_raw_border_merged, tri)
		&& !ee::Math2D::IsPolygonIntersectPolygon(m_fine_border, tri);
}

// 已经是算出来的
void ExtractOutlineFine::StartPosExplore(const sm::vec2& p0, const sm::vec2& p1, 
										 float& start_scale, float& end_scale, 
										 sm::vec2& mid, float& score) const
{
	for (int s = 1; s < STEPS_COUNT; ++s)
	{
		float step = STEPS[s];

		// ori
		float ori_end_scale = end_scale;
		float ori_score = 0;
		sm::vec2 ori_mid;
		EndPosExplore(step, p0, p1, start_scale, end_scale, ori_mid, ori_score);
		// sub
		float sub_start_scale = start_scale - step;
		float sub_end_scale = end_scale;
		float sub_score = 0;
		sm::vec2 sub_mid;
		if (sub_start_scale <= 1 && sub_start_scale < end_scale) {
			EndPosExplore(step, p0, p1, sub_start_scale, sub_end_scale, sub_mid, sub_score);
		}
		// add
		float add_start_scale = start_scale + step;
		float add_end_scale = end_scale;
		float add_score = 0;
		sm::vec2 add_mid;
		if (add_start_scale <= 1 && add_start_scale < end_scale) {
			EndPosExplore(step, p0, p1, add_start_scale, add_end_scale, add_mid, add_score);
		}
		// decide direction
		if (ori_score > sub_score && ori_score > add_score) {
			if (ori_score > score) {
				score = ori_score;
				mid = ori_mid;
			}
			continue;
		}

		if (sub_score <= score && add_score <= score) {
			continue;
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
void ExtractOutlineFine::EndPosExplore(float step, const sm::vec2& p0, const sm::vec2& p1, float start_scale, 
									   float& end_scale, sm::vec2& mid, float& score) const
{
 	sm::vec2 start_pos = p0 + (p1-p0)*start_scale,
 		end_pos = p0 + (p1-p0)*end_scale;
	float curr_score;
	sm::vec2 curr_mid;
	MidPosExplore(start_pos, end_pos, curr_mid, curr_score);
	if (curr_score <= score) {
		return;
	} else {
		mid = curr_mid;
		score = curr_score;
	}

	float curr_step = step;
	// sub
	float sub_end_scale = end_scale - curr_step;
	float sub_score = 0;
	sm::vec2 sub_mid;
	if (sub_end_scale <= 1 && sub_end_scale > start_scale) {
		sm::vec2 e = p0 + (p1-p0)*sub_end_scale;
		MidPosExplore(start_pos, e, sub_mid, sub_score);
	}
	// add
	float add_end_scale = end_scale + curr_step;
	float add_score = 0;
	sm::vec2 add_mid;
	if (add_end_scale <= 1 && add_end_scale > start_scale) {
		sm::vec2 e = p0 + (p1-p0)*add_end_scale;
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
		sm::vec2 curr_mid;
		float curr_score;
		sm::vec2 e = p0 + (p1-p0)*curr_end_scale;
		MidPosExplore(start_pos, e, curr_mid, curr_score);
		if (curr_score > score) {
			score = curr_score;
			end_scale = curr_end_scale;
			mid = curr_mid;
			curr_end_scale += curr_step;
		} else {
			break;
		}
	}
}

void ExtractOutlineFine::MidPosExplore(const sm::vec2& start, const sm::vec2& end, 
									   sm::vec2& mid, float& score) const
{
	//// order
	//// 3 4 5
	//// 2   6
	//// 1 0 7
	//static const int DIR_COUNT = 8;
	//static const sm::vec2 DIRS[DIR_COUNT] = {
	//	sm::vec2( 0, -1),
	//	sm::vec2(-1, -1),
	//	sm::vec2(-1,  0),
	//	sm::vec2(-1,  1),
	//	sm::vec2( 0,  1),
	//	sm::vec2( 1,  1),
	//	sm::vec2( 1,  0),
	//	sm::vec2( 1, -1),
	//};

	static const int STEPS_COUNT = 6;
	static const float STEPS[STEPS_COUNT] = {128, 64, 32, 16, 8, 4};

	mid = (start + end) * 0.5f;
	score = 0;
	for (int i = 0; i < STEPS_COUNT; ++i) {
		float step = STEPS[i];

		while (true)
		{
			float old_score = score;
			for (int j = 0, m = m_rotate_lut.size(); j < m; ++j)
			{
				sm::vec2 offset = m_rotate_lut[j] * step;
				sm::vec2 curr_mid = mid + offset;
				float area = ee::Math2D::GetTriangleArea(start, curr_mid, end);
				if (area > score &&
					sm::is_point_in_area(curr_mid, m_fine_border)) {

						// !ee::Math2D::isPointInArea(curr_mid, m_raw_border_merged)

						if (IsAddTriLeagal(start, end, curr_mid)) {
							score = area;
							mid = curr_mid;
						}
				}
			}
			if (score <= old_score + 1) {
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

	float area_limit = ee::Math2D::GetPolygonArea(m_fine_border) * tolerance;

	bool success = false;
	do {
		success = false;
		for (int i = 0, n = m_fine_border.size(); i < n; ++i)
		{
			const sm::vec2& curr = m_fine_border[i];
			const sm::vec2& prev = m_fine_border[(i+m_fine_border.size()-1)%m_fine_border.size()];
			const sm::vec2& next = m_fine_border[(i+1)%m_fine_border.size()];
			sm::vec2 s0 = curr - prev, s1 = next - curr;
			// area increase & perimeter decrease
			if (s0.Cross(s1) >= 0) 
			{
				float a = ee::Math2D::GetTriangleArea(curr, prev, next);
				if (a < area_limit && IsAddTriLeagal(prev, next, curr))
				{
					m_fine_border.erase(m_fine_border.begin()+i);
					success = true;
					break;
				}
			} 
			// area decrease & perimeter decrease
			else 
			{
				if (IsCutTriLegal(curr, prev, next)) {
					m_fine_border.erase(m_fine_border.begin()+i);
					success = true;
					break;
				}
			}
		}
	} while (success && m_fine_border.size() > 3);
}

void ExtractOutlineFine::ReduceEdge(float area_tol, float perimeter_tol)
{
	if (m_fine_border.size() <= 3) {
		return;
	}

	float area_limit = ee::Math2D::GetPolygonArea(m_fine_border) * area_tol;
	float perimeter_limit = ee::Math2D::GetPolygonPerimeter(m_fine_border) * perimeter_tol;
	bool success = false;
	do {
		ReduceNode(area_tol);
		if (m_fine_border.size() <= 3) {
			break;
		}

		success = false;
		for (int i = 0, n = m_fine_border.size(); i < n; ++i)
		{
			const sm::vec2& start = m_fine_border[i];
			const sm::vec2& start_prev = m_fine_border[(i+m_fine_border.size()-1)%m_fine_border.size()];
			const sm::vec2& end = m_fine_border[(i+1)%m_fine_border.size()];
			const sm::vec2& end_next = m_fine_border[(i+2)%m_fine_border.size()];
			assert(start != end);
			if (start_prev == end_next) {
				continue;
			}
			
			sm::vec2 intersect;
			bool cross = sm::intersect_line_line(start_prev, start, end, end_next, &intersect);
			if (!cross) {
				continue;
			}

			bool inside_s = ee::Math2D::IsBetween(start_prev.x, start.x, intersect.x) 
				&& ee::Math2D::IsBetween(start_prev.y, start.y, intersect.y);
			bool inside_e = ee::Math2D::IsBetween(end.x, end_next.x, intersect.x) 
				&& ee::Math2D::IsBetween(end.y, end_next.y, intersect.y);
			// add new triangle, area increase & perimeter increase
			if (!inside_s && !inside_e) 
			{
				float a = ee::Math2D::GetTriangleArea(intersect, start, end);
				float len = sm::dis_pos_to_pos(intersect, start) 
					+ sm::dis_pos_to_pos(intersect, end)
					- sm::dis_pos_to_pos(start, end);
				if (a < area_limit && len < perimeter_limit && 
					IsAddTriLeagal(start, end, intersect))
				{
					m_fine_border[i] = intersect;
					m_fine_border.erase(m_fine_border.begin()+((i+1)%m_fine_border.size()));
					success = true;
					break;
				}
			} 
			// cut triangle, perimeter decrease
			else 
			{
				assert(!inside_s || !inside_e);
// 				if (inside_s && inside_e) {
// 					break;
// 				}

				bool is_inside;
				if (inside_s) {
					is_inside = sm::is_turn_right(start, intersect, end_next);
				} else {
					is_inside = sm::is_turn_left(end, intersect, start_prev);
				}
				if (is_inside || ee::Math2D::GetTriangleArea(intersect, start, end) < area_limit) {
					if (IsCutTriLegal(intersect, start, end)) {
						if (inside_s) {
							m_fine_border[i] = intersect;
							m_fine_border.erase(m_fine_border.begin()+((i+1)%m_fine_border.size()));
						} else {
							m_fine_border[(i+1)%m_fine_border.size()] = intersect;
							m_fine_border.erase(m_fine_border.begin()+i);
						}
						success = true;
						break;
					}
				}
			}
		}
	} while (success && m_fine_border.size() > 3);
}

void ExtractOutlineFine::InitRotateLUT()
{
	static const int DIR_COUNT = 32;

	m_rotate_lut.clear();
	m_rotate_lut.reserve(DIR_COUNT);
	for (int i = 0; i < DIR_COUNT; ++i) 
	{
		float angle = 2 * SM_PI * i / DIR_COUNT;
		sm::vec2 dir(cos(angle), sin(angle));
		m_rotate_lut.push_back(dir);
	}
}

}