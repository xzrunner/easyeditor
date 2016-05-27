#ifndef _SPRITE2_REF_COUNT_OBJ_INL_
#define _SPRITE2_REF_COUNT_OBJ_INL_

namespace s2
{

inline
RefCountObj::RefCountObj()
	: m_ref_count(1)
{
}

inline
RefCountObj::RefCountObj(const RefCountObj& obj)
	: m_ref_count(1)
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

template<class T>
void rcobj_assign(T*& lhs, T*& rhs)
{
	if (lhs == rhs) {
		return;
	}

	if (lhs) {
		lhs->RemoveReference();
	}
	lhs = rhs;
	if (rhs) {
		rhs->AddReference();
	}
}

}

#endif // _SPRITE2_REF_COUNT_OBJ_INL_