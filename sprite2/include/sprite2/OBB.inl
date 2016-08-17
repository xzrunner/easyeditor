#ifndef _SPRITE2_OBB_INL_
#define _SPRITE2_OBB_INL_

#include <SM_Test.h>
#include <SM_Calc.h>

namespace s2
{

inline
OBB::OBB()
	: m_angle(0)
{
}

inline 
bool OBB::IsContain(const sm::vec2& pos) const
{
	sm::vec2 position = sm::rotate_vector(pos - m_position, -m_angle);
	return sm::is_point_in_rect(position, m_rect);
}

inline 
bool OBB::IsContain(const sm::rect& rect) const
{
	return IsContain(sm::vec2(rect.xmin, rect.ymin))
		&& IsContain(sm::vec2(rect.xmax, rect.ymin))
		&& IsContain(sm::vec2(rect.xmax, rect.ymax))
		&& IsContain(sm::vec2(rect.xmin, rect.ymax));
}

inline 
bool OBB::IsIntersect(const sm::rect& rect) const
{
	std::vector<sm::vec2> convex0(4), convex1(4);
	convex0[0] = sm::rotate_vector(sm::vec2(m_rect.xmin, m_rect.ymin), m_angle) + m_position;
	convex0[1] = sm::rotate_vector(sm::vec2(m_rect.xmax, m_rect.ymin), m_angle) + m_position;
	convex0[2] = sm::rotate_vector(sm::vec2(m_rect.xmax, m_rect.ymax), m_angle) + m_position;
	convex0[3] = sm::rotate_vector(sm::vec2(m_rect.xmin, m_rect.ymax), m_angle) + m_position;
	convex1[0] = sm::vec2(rect.xmin, rect.ymin);
	convex1[1] = sm::vec2(rect.xmax, rect.ymin);
	convex1[2] = sm::vec2(rect.xmax, rect.ymax);
	convex1[3] = sm::vec2(rect.xmin, rect.ymax);
	return is_convex_intersect_convex(convex0, convex1);
}

inline
void OBB::CombineTo(sm::rect& r) const
{
	r.Combine(sm::rotate_vector(sm::vec2(m_rect.xmin, m_rect.ymin), m_angle) + m_position);
	r.Combine(sm::rotate_vector(sm::vec2(m_rect.xmax, m_rect.ymin), m_angle) + m_position);
	r.Combine(sm::rotate_vector(sm::vec2(m_rect.xmax, m_rect.ymax), m_angle) + m_position);
	r.Combine(sm::rotate_vector(sm::vec2(m_rect.xmin, m_rect.ymax), m_angle) + m_position);
}

inline
void OBB::Build(const sm::rect& r, const sm::vec2& pos, float angle, 
				const sm::vec2& scale, const sm::vec2& shear, const sm::vec2& offset)
{
	m_rect = r;
	if (scale != sm::vec2(1, 1)) {
		m_rect.Scale(scale.x, scale.y);
	}
	if (shear != sm::vec2(0, 0)) {
		m_rect.Shear(shear.x, shear.y);
	}
	m_position = pos;
	if (offset != sm::vec2(0, 0)) {
		m_position += sm::rotate_vector(-offset, angle) + offset;
	}
	m_angle = angle;
}

inline 
void OBB::SetTransform(const sm::vec2& position, const sm::vec2& offset, float angle) 
{
	m_position = position + (sm::rotate_vector(-offset, angle) + offset);
	m_angle = angle;
}

inline
void OBB::GetBoundPos(std::vector<sm::vec2>& bound) const
{
	bound.resize(4);
	bound[0] = sm::rotate_vector(sm::vec2(m_rect.xmin, m_rect.ymin), m_angle) + m_position;
	bound[1] = sm::rotate_vector(sm::vec2(m_rect.xmax, m_rect.ymin), m_angle) + m_position;
	bound[2] = sm::rotate_vector(sm::vec2(m_rect.xmax, m_rect.ymax), m_angle) + m_position;
	bound[3] = sm::rotate_vector(sm::vec2(m_rect.xmin, m_rect.ymax), m_angle) + m_position;
}

}

#endif // _SPRITE2_OBB_INL_