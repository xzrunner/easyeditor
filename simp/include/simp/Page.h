#ifndef _SIMP_PAGE_H_
#define _SIMP_PAGE_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class Allocator;
class ImportStream;

class Page : private cu::Uncopyable
{
public:
	Page(Allocator& alloc, ImportStream& is, int begin_id, int end_id);
	~Page();

	static int Size();

private:
	void Init(Allocator& alloc, ImportStream& is);

	static void* CreateNode(uint8_t type, Allocator& alloc, ImportStream& is);

private:
	int m_begin_id, m_end_id;	

	uint8_t* m_types;
	void** m_nodes;

}; // Page

}

#endif // _SIMP_PAGE_H_