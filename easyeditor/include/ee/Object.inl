#ifndef _EASYEDITOR_OBJECT_INL_
#define _EASYEDITOR_OBJECT_INL_

namespace ee
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

template<class T>
void obj_assign(T*& left, T*& right)
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

}

#endif // _EASYEDITOR_OBJECT_INL_