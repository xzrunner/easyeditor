#ifndef _D2D_OBJECT_INL_
#define _D2D_OBJECT_INL_

namespace d2d
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
obj_assign(Object*& left, Object* right)
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

} // d2d

#endif // _D2D_OBJECT_INL_