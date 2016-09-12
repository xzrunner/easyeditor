#ifndef _SIMP_ALLOCATOR_H_
#define _SIMP_ALLOCATOR_H_

#include <CU_Uncopyable.h>

namespace simp
{

class Allocator : private cu::Uncopyable
{
public:
	Allocator(int cap);
	~Allocator();

	void* Alloc(int sz);

	bool Empty() const { return m_cap == 0; }

private:
	char* m_buffer;
	int m_cap;

}; // Allocator

}

#endif // _SIMP_ALLOCATOR_H_