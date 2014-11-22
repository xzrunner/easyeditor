#ifndef _D2D_OBJECT_H_
#define _D2D_OBJECT_H_

namespace d2d
{

class Object
{
public:
	Object();
	virtual ~Object() {}

	virtual void Retain();
	virtual void Release();

private:
	int m_count;

}; // Object

inline void obj_assign(Object*& left, Object* right);

}

#include "Object.inl"

#endif _D2D_OBJECT_H_