#include "ExtractOutlineFine.h"

namespace eimage
{

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

void ExtractOutlineFine::AddOneNode(int idx, float r_decrease, d2d::Vector& new_start,
									d2d::Vector& new_end, d2d::Vector& new_node, float& decrease) const
{
	decrease = 0;
	const d2d::Vector& curr = m_fine_border[idx];
	const d2d::Vector& next = m_fine_border[(idx+1)%m_fine_border.size()];
	float len = d2d::Math::getDistance(curr, next);
	for (float idx_s = 0; idx_s < len; idx_s+=5) {
		d2d::Vector s = curr + (next-curr)*(idx_s/len);
		for (float idx_e = 0; idx_e < len-idx_s; idx_e+=5) {
			d2d::Vector e = next + (curr-next)*(idx_e/len);
			// todo new_node not need to traverse all raw_border
			for (int i = 0, n = m_raw_border.size(); i < n; ++i) {
				const d2d::Vector& _new = m_raw_border[i];
				float a = d2d::Math::GetTriangleArea(s, _new, e);
				if (a > decrease && a > r_decrease) {
					//					if (IsSegmentLegal(m_raw_border, s, _new, e)) {

					d2d::Vector c = d2d::Math::getTriGravityCenter(s, e, _new);
					if (!d2d::Math::isPointInArea(c, m_fine_border)) {
						continue;
					}

					if (IsSegmentLegalNew(s, _new, e, _new)) {
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

}