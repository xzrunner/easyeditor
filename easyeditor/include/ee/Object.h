#ifndef _EASYEDITOR_OBJECT_H_
#define _EASYEDITOR_OBJECT_H_

#include <stddef.h>

namespace ee
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

#endif _EASYEDITOR_OBJECT_H_