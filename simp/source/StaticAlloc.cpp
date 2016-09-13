#include "StaticAlloc.h"
#include "Allocator.h"
#include "simp_define.h"

namespace simp
{

SINGLETON_DEFINITION(StaticAlloc)

StaticAlloc::StaticAlloc()
{
}

Allocator* StaticAlloc::Create()
{
	Allocator* ret = NULL;
	if (!m_freelist.empty()) {
		ret = m_freelist.back();
		m_freelist.pop_back();
	} else {
		return new Allocator(PAGE_SIZE);
	} 
	return ret;
}

void StaticAlloc::Release(Allocator* alloc)
{
	if (alloc) {
		m_freelist.push_back(alloc);
	}
}

}