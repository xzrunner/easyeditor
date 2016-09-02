#include "Allocator.h"

#include <stddef.h>

namespace simp
{

Allocator::Allocator(int cap)
	: m_cap(cap)
{
	m_buffer = new char[cap];
}

Allocator::~Allocator()
{
	delete[] m_buffer;
}

void* Allocator::Alloc(int sz)
{
	if (sz <= 0) {
		return NULL;
	}
	if (m_cap < sz) {
		return NULL;
	}
	void* ret = m_buffer;
	m_buffer += sz;
	m_cap -= sz;
	return ret;
}

}