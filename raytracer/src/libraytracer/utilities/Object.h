#ifndef _RT_OBJECT_H_
#define _RT_OBJECT_H_

namespace rt
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

inline void obj_assign(const Object*& left, const Object* right);

}

#include "Object.inl"

#endif _RT_OBJECT_H_