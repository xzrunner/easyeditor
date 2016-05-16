#ifndef _SPRITE2_OBJECT_H_
#define _SPRITE2_OBJECT_H_

namespace s2
{

class Object
{
public:
	Object() : m_count(1) {}
	virtual ~Object() {}

	virtual void Retain() const { 
		++m_count;
	}
	virtual void Release() const {
		if (--m_count <= 0) {
			delete this;
		}
	}

private:
	mutable int m_count;

}; // Object

}

#endif // _SPRITE2_OBJECT_H_