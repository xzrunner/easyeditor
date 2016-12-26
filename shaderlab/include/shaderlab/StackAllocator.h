#ifndef _SHADERLAB_STACK_ALLOCATOR_H_
#define _SHADERLAB_STACK_ALLOCATOR_H_

#include <stdint.h>
#include <stddef.h>

namespace sl
{

class StackAllocator
{
public:
	void Reserve(int cap) {
		if (m_sz == 0 && cap > m_cap) {
			if (m_buf) {
				delete m_buf;
			}
			m_buf = new uint8_t[cap];
			m_cap = cap;
		}
	}

	void* Alloc(int sz) {
		if (sz <= m_cap - m_sz) {
			void* ret = m_buf + m_sz;
			m_sz += sz;
			return ret;
		} else {
			return NULL;
		}
	}

	void Free(void* mem) {
		if (mem >= m_buf && mem < m_buf + m_cap) {
			m_sz = (intptr_t)mem - (intptr_t)m_buf;
		}
	}

	static StackAllocator* Instance();

private:
	StackAllocator() : m_buf(NULL), m_cap(0), m_sz(0) {}

private:
	uint8_t* m_buf;
	int m_cap;
	int m_sz;

private:
	static StackAllocator* m_instance;

}; // StackAllocator

}

#endif // _SHADERLAB_STACK_ALLOCATOR_H_