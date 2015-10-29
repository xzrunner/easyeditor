#ifndef _D2D_OBJECT_H_
#define _D2D_OBJECT_H_

namespace d2d
{

class Object
{
public:
	Object();
	virtual ~Object() {}

	virtual void Retain() const;
	virtual void Release() const;

private:
	mutable int m_count;

}; // Object

template<class T>
inline void obj_assign(T*& left, T*& right)
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

template<class T>
struct ReleaseObjectFunctor
{
	void operator()(const T* pt) const {
		pt->Release(), pt = NULL;
	}
}; // ReleaseObjectFunctor

template<class T>
struct RetainObjectFunctor
{
	void operator()(const T* pt) const {
		pt->Retain();
	}
}; // RetainObjectFunctor

}

#include "Object.inl"

#endif _D2D_OBJECT_H_