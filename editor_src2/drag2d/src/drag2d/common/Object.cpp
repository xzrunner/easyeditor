#include "Object.h"

namespace d2d
{

Object::Object()
{
	m_count = 1;
}

Object::~Object()
{
}

int Object::getReferenceCount() const
{
	return m_count;
}

void Object::retain()
{
	++m_count;
}

void Object::release()
{
	if (--m_count <= 0)
		delete this;
}

} // d2d