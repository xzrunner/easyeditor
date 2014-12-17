#ifndef _RT_OBJECT_INL_
#define _RT_OBJECT_INL_

namespace rt
{

inline 
Object::Object()
{
	m_count = 1;
}

inline void 
Object::Retain() const
{
	++m_count;
}

inline void 
Object::Release() const
{
	if (--m_count <= 0) {
		delete this;
	}
}

inline void 
obj_assign(const Object*& left, const Object* right)
{
	if (left == right) {
		return;
	}

	if (left) {
		left->Release();
	}
	left = right;
	if (right) {
		right->Retain();
	}
}

} // rt

#endif // _RT_OBJECT_INL_