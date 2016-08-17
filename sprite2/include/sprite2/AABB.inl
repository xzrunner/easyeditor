#ifndef _SPRITE2_AABB_INL_
#define _SPRITE2_AABB_INL_

#include <SM_Test.h>
#include <SM_Calc.h>

namespace s2
{

inline 
bool AABB::IsContain(const sm::vec2& pos) const
{
	return sm::is_point_in_rect(pos - m_position, m_rect);
}

inline 
bool AABB::IsContain(const sm::rect& rect) const
{
	sm::rect r(rect);
	r.Translate(-m_position);
	return sm::is_rect_contain_rect(m_rect, r);
}

inline 
bool AABB::IsIntersect(const sm::rect& rect) const
{
	sm::rect r(rect);
	r.Translate(-m_position);
	return sm::is_rect_intersect_rect(m_rect, r);	
}

inline
void AABB::CombineTo(sm::rect& r) const
{
	sm::rect me(m_rect);
	me.Translate(m_position);
	r.Combine(me);
}

inline
void AABB::Build(const sm::rect& r, const sm::vec2& pos, float angle, 
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
}

inline
void AABB::SetTransform(const sm::vec2& position, const sm::vec2& offset, float angle)
{
	m_position = position + (sm::rotate_vector(-offset, angle) + offset);
}

inline
void AABB::GetBoundPos(std::vector<sm::vec2>& bound) const
{
	bound.resize(4);
	bound[0] = sm::vec2(m_rect.xmin, m_rect.ymin) + m_position;
	bound[1] = sm::vec2(m_rect.xmax, m_rect.ymin) + m_position;
	bound[2] = sm::vec2(m_rect.xmax, m_rect.ymax) + m_position;
	bound[3] = sm::vec2(m_rect.xmin, m_rect.ymax) + m_position;
}

}

#endif // _SPRITE2_AABB_INL_