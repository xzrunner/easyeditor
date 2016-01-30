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
void AABB::InitFromRect(const Rect& rect)
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
void AABB::MakeInfinite()
{
	m_rect.MakeInfinite();
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
void AABB::Combine(const Vector& pos)
{
	m_rect.Combine(pos - m_position);
}

inline 
void AABB::Combine(const Rect& rect)
{
	Rect r(rect);
	r.Translate(-m_position);
	m_rect.Combine(r);
}

inline
float AABB::Area() const
{
	return m_rect.Width() * m_rect.Height();
}

inline
float AABB::Width() const
{
	return m_rect.Width();
}

inline
float AABB::Height() const
{
	return m_rect.Height();
}

inline
Vector AABB::Center() const
{
	return m_position;
}

inline
void AABB::GetBoundPos(std::vector<Vector>& bound) const
{
	if (m_rect.IsValid()) {
		bound.clear();
		bound.push_back(Vector(m_rect.xmin, m_rect.ymin) + m_position);
		bound.push_back(Vector(m_rect.xmax, m_rect.ymin) + m_position);
		bound.push_back(Vector(m_rect.xmax, m_rect.ymax) + m_position);
		bound.push_back(Vector(m_rect.xmin, m_rect.ymax) + m_position);
	}
}

}

#endif // _EASYEDITOR_AABB_INL_
