#ifndef _SPRITE2_REF_COUNT_OBJ_INL_
#define _SPRITE2_REF_COUNT_OBJ_INL_

namespace s2
{

inline
RefCountObj::RefCountObj()
	: m_ref_count(0)
	, m_shareable(true)
{
}

inline
RefCountObj::RefCountObj(const RefCountObj& obj)
	: m_ref_count(0)
	, m_shareable(true)
{
}

inline
RefCountObj& RefCountObj::operator = (const RefCountObj& obj)
{
	return *this;
}

inline
RefCountObj::~RefCountObj()
{
}

inline
void RefCountObj::AddReference()
{
	++m_ref_count;
}

inline
void RefCountObj::RemoveReference()
{
	if (--m_ref_count <= 0) {
		delete this;
	}
}

inline
void RefCountObj::MarkUnshareable()
{
	m_shareable = false;
}

inline
bool RefCountObj::IsShareable() const
{
	return m_shareable;
}

inline
bool RefCountObj::IsSheared() const
{
	return m_ref_count > 1;
}

}

#endif // _SPRITE2_REF_COUNT_OBJ_INL_