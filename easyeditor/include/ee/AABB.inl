#ifndef _EASYEDITOR_AABB_INL_
#define _EASYEDITOR_AABB_INL_

namespace ee
{

inline
AABB::AABB()
{
}

inline
AABB::AABB(const AABB& aabb)
	: m_rect(aabb.m_rect)
	, m_position(aabb.m_position)
{
}

inline
AABB* AABB::Clone() const
{
	return new AABB(*this);
}

inline 
void AABB::InitFromRect(const sm::rect& rect)
{
	m_rect = rect;
	m_position.Set(0, 0);
}

inline 
bool AABB::IsValid() const
{
	return m_rect.IsValid();
}

inline 
void AABB::MakeEmpty()
{
	m_rect.MakeEmpty();
}

inline
void AABB::SetMirror(bool xmirror, bool ymirror)
{
	if (xmirror) {
		m_rect.xmin = -m_rect.xmin;
		m_rect.xmax = -m_rect.xmax;
		std::swap(m_rect.xmin, m_rect.xmax);
	}
	if (ymirror) {
		m_rect.ymin = -m_rect.ymin;
		m_rect.ymax = -m_rect.ymax;
		std::swap(m_rect.ymin, m_rect.ymax);
	}
}

inline 
void AABB::Combine(const sm::vec2& pos)
{
	m_rect.Combine(pos - m_position);
}

inline 
void AABB::Combine(const sm::rect& rect)
{
	sm::rect r(rect);
	r.Translate(-m_position);
	m_rect.Combine(r);
}

inline
float AABB::Area() const
{
	sm::vec2 sz = m_rect.Size();
	return sz.x * sz.y;
}

inline
float AABB::Width() const
{
	return m_rect.Size().x;
}

inline
float AABB::Height() const
{
	return m_rect.Size().y;
}

inline
sm::vec2 AABB::Center() const
{
	return m_position;
}

inline
void AABB::GetBoundPos(std::vector<sm::vec2>& bound) const
{
	if (m_rect.IsValid()) {
		bound.clear();
		bound.push_back(sm::vec2(m_rect.xmin, m_rect.ymin) + m_position);
		bound.push_back(sm::vec2(m_rect.xmax, m_rect.ymin) + m_position);
		bound.push_back(sm::vec2(m_rect.xmax, m_rect.ymax) + m_position);
		bound.push_back(sm::vec2(m_rect.xmin, m_rect.ymax) + m_position);
	}
}

}

#endif // _EASYEDITOR_AABB_INL_
