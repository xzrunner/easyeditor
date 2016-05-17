#ifndef _EASYEDITOR_OBB_INL_
#define _EASYEDITOR_OBB_INL_

namespace ee
{

inline
OBB::OBB()
{
	m_angle = 0.0f;
}

inline
OBB::OBB(const OBB& obb)
	: m_rect(obb.m_rect)
	, m_position(obb.m_position)
	, m_angle(obb.m_angle)
{
}

inline
OBB* OBB::Clone() const
{
	return new OBB(*this);
}

inline 
void OBB::InitFromRect(const sm::rect& rect)
{
	m_rect = rect;
	m_position.Set(0, 0);
	m_angle = 0;
}

inline 
bool OBB::IsValid() const
{
	return m_rect.IsValid();
}

inline 
void OBB::MakeEmpty()
{
	m_rect.MakeEmpty();
}

inline
void OBB::SetMirror(bool xmirror, bool ymirror)
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
void OBB::Combine(const sm::vec2& pos)
{
	if (m_rect.IsValid()) {
		TransToAABB();
	}
	m_rect.Combine(pos - m_position);
}

inline 
void OBB::Combine(const sm::rect& rect)
{
	if (m_rect.IsValid()) {
		TransToAABB();
	}
	sm::rect r(rect);
	r.Translate(-m_position);
	m_rect.Combine(r);
}

inline
float OBB::Area() const
{
	sm::vec2 sz = m_rect.Size();
	return sz.x * sz.y;
}

inline
float OBB::Width() const
{
	return m_rect.Size().x;
}

inline
float OBB::Height() const
{
	return m_rect.Size().y;
}

inline
sm::vec2 OBB::Center() const
{
	return m_position;
}

}

#endif // _EASYEDITOR_OBB_INL_
