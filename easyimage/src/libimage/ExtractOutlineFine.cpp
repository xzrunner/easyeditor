#include "ExtractOutlineFine.h"

namespace eimage
{

static const int STEPS_COUNT = 3;
static const float STEPS[] = {1/8.0f, 1/16.0f, 1/32.0f};

ExtractOutlineFine::ExtractOutlineFine(const std::vector<d2d::Vector>& raw_border, 
									   const std::vector<d2d::Vector>& raw_border_merged)
	: m_raw_border(raw_border)
	, m_raw_border_merged(raw_border_merged)
{
}

void ExtractOutlineFine::Trigger(float tolerance)
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

	bool success = false;
	do {
		success = false;

		float area = d2d::Math::GetPolygonArea(m_fine_border);

		// remove one node
		d2d::Vector r_new0, r_new1;
		int r_idx = 0;
		float r_area_decrease = 0;
		for (int i = 0, n = m_fine_border.size(); i < n; ++i) {
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
		for (int i = 0, n = m_fine_border.size(); i < n; ++i) {
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
				int sz = m_fine_border.size();
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
	} while (success && m_fine_border.size() < 8);
}

void ExtractOutlineFine::RemoveOneNode(int idx, d2d::Vector& new0, d2d::Vector& new1, float& decrease) const
{
	decrease = 0;

	const d2d::Vector& curr = m_fine_border[idx];
	const d2d::Vector& prev = m_fine_border[(idx-1)%m_fine_border.size()];
	const d2d::Vector& next = m_fine_border[(idx+1)%m_fine_border.size()];
	float len_prev = d2d::Math::getDistance(curr, prev),
		len_next = d2d::Math::getDistance(curr, next);
	float area_max = 0;
	float idx_e = 0;
	for (float i = 0; i < len_prev; i+=1) {
		d2d::Vector s = prev + (curr-prev)*(i/len_prev);
		for (float j = idx_e; j < len_next; j+=1) {
			d2d::Vector e = curr + (next-curr)*(j/len_next);
			if (IsSegmentLegal(s, curr, e)) {
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

// todo Ì«Ð¡±ßµÄÌ½Ë÷¾¡¿ìÖÕÖ¹µô
void ExtractOutlineFine::AddOneNode(int idx, float r_decrease, d2d::Vector& new_start,
									d2d::Vector& new_end, d2d::Vector& new_node, float& decrease) const
{
	decrease = 0;
	const d2d::Vector& curr = m_fine_border[idx];
	const d2d::Vector& next = m_fine_border[(idx+1)%m_fine_border.size()];
	float max_score = 0;
	d2d::Vector best_node;
	// init region
	float start_scale = 0, end_scale = 1;
	assert(STEPS_COUNT > 0);
	float step = STEPS[0];
	for ( ; start_scale < 1; start_scale += step) {
		d2d::Vector start_pos = curr + (next-curr)*start_scale;
		for ( ; end_scale > start_scale; end_scale -= step) {
			d2d::Vector end_pos = curr + (next-curr)*end_scale;
			d2d::Vector mid_pos;
			float score;
			MidPosExplore(start_pos, end_pos, mid_pos, score);
			if (score > max_score) {
				max_score = score;
				best_node = mid_pos;
			}
		}
	}
	// refine region

}

//void ExtractOutlineFine::AddOneNode(int idx, float r_decrease, d2d::Vector& new_start,
//									d2d::Vector& new_end, d2d::Vector& new_node, float& decrease) const
//{
//	decrease = 0;
//	const d2d::Vector& curr = m_fine_border[idx];
//	const d2d::Vector& next = m_fine_border[(idx+1)%m_fine_border.size()];
//	float len = d2d::Math::getDistance(curr, next);
//	for (float idx_s = 0; idx_s < len; idx_s+=5) {
//		d2d::Vector s = curr + (next-curr)*(idx_s/len);
//		for (float idx_e = 0; idx_e < len-idx_s; idx_e+=5) {
//			d2d::Vector e = next + (curr-next)*(idx_e/len);
//			// todo new_node not need to traverse all raw_border
//			for (int i = 0, n = m_raw_border.size(); i < n; ++i) {
//				const d2d::Vector& _new = m_raw_border[i];
//				float a = d2d::Math::GetTriangleArea(s, _new, e);
//				if (a > decrease && a > r_decrease) {
//					//					if (IsSegmentLegal(m_raw_border, s, _new, e)) {
//
//					d2d::Vector c = d2d::Math::getTriGravityCenter(s, e, _new);
//					if (!d2d::Math::isPointInArea(c, m_fine_border)) {
//						continue;
//					}
//
//					if (IsSegmentLegalNew(s, _new, e, _new)) {
//						decrease = a;
//						new_start = s;
//						new_end = e;
//						new_node = _new;
//					}
//				}
//			}
//		}
//	}
//}

bool ExtractOutlineFine::IsSegmentLegal(const d2d::Vector& p0, const d2d::Vector& p1, 
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

bool ExtractOutlineFine::IsSegmentLegalNew(const d2d::Vector& s0, const d2d::Vector& e0, 
										   const d2d::Vector& s1, const d2d::Vector& e1) const
{
	for (int i = 0, n = m_raw_border_merged.size(); i < n; ++i) {
		const d2d::Vector& s = m_raw_border_merged[i];
		const d2d::Vector& e = m_raw_border_merged[(i+1)%m_raw_border_merged.size()];
		if (d2d::Math::IsTwoLineIntersect(s, e, s0, e0) || d2d::Math::IsTwoLineIntersect(s, e, s1, e1)) {
			return false;
		}
	}
	return true;
}

void ExtractOutlineFine::EndPosExplore(const d2d::Vector& p0, const d2d::Vector& p1, float start_scale, 
									   float& end_scale, d2d::Vector& mid, float& score) const
{
	d2d::Vector start_pos = p0 + (p1-p0)*start_scale,
		end_pos = p0 + (p1-p0)*end_scale;
	float best_end_scale = end_scale;
	float best_score;
	d2d::Vector best_mid;
	MidPosExplore(start_pos, end_pos, best_mid, best_score);

	for (int i = 1; i < STEPS_COUNT; ++i)
	{
		float step = STEPS[i];
		// sub
		float sub_end_scale = end_scale - step;
		float sub_score = 0;
		d2d::Vector sub_mid;
		if (sub_end_scale <= 1 && sub_end_scale > start_scale) {
			d2d::Vector e = p0 + (p1-p0)*sub_end_scale;
			MidPosExplore(start_pos, e, sub_mid, sub_score);
		}
		// add
		float add_end_scale = end_scale + step;
		float add_score = 0;
		d2d::Vector add_mid;
		if (add_end_scale <= 1 && add_end_scale > start_scale) {
			d2d::Vector e = p0 + (p1-p0)*add_end_scale;
			MidPosExplore(start_pos, e, add_mid, add_score);
		}
		// final
		if (sub_score <= best_score && add_score <= best_score) {
			continue;
		} else if (sub_score > best_score) {
			step = -STEPS[i];
			best_end_scale = sub_end_scale;
			best_score = sub_score;
			best_mid = sub_mid;
		} else {
			step = STEPS[i];
			best_end_scale = add_end_scale;
			best_score = add_score;
			best_mid = add_mid;
		}

		float curr_end_scale = best_end_scale + step;
		while (curr_end_scale <= 1 && curr_end_scale > start_scale)
		{
			float curr_score = 0;
			d2d::Vector curr_mid;
			d2d::Vector e = p0 + (p1-p0)*curr_end_scale;
			MidPosExplore(start_pos, e, curr_mid, curr_score);
			if (curr_score > best_score) {
				best_end_scale = curr_end_scale;
				best_score = curr_score;
				best_mid = curr_mid;
				curr_end_scale += step;
			} else {
				break;
			}
		}
	}
}

void ExtractOutlineFine::MidPosExplore(const d2d::Vector& start, const d2d::Vector& end, 
									   d2d::Vector& mid, float& score) const
{
	
}

}