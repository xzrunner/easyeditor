#ifndef _SIMP_ALLOCATOR_H_
#define _SIMP_ALLOCATOR_H_

#include <CU_Uncopyable.h>

namespace simp
{

class Package : private cu::Uncopyable
{
public:
	Package();
	~Package();

private:
	int m_begin_id, m_end_id;

//	uint8_t** m_sprs;

}; // Package

}

#endif // _SIMP_ALLOCATOR_H_