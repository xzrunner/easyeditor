#ifndef _CLASS_UTILITY_REF_COUNT_OBJ_INL_
#define _CLASS_UTILITY_REF_COUNT_OBJ_INL_

namespace cu
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
void RefCountObj::AddReference() const
{
	++m_ref_count;
}

inline
void RefCountObj::RemoveReference() const
{
	if (--m_ref_count <= 0) {
		delete this;
	}
}

}

#endif // _CLASS_UTILITY_REF_COUNT_OBJ_INL_