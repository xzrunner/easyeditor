#ifndef _SPRITE_IMPORT_STATIC_ALLOC_H_
#define _SPRITE_IMPORT_STATIC_ALLOC_H_

#include <CU_Singleton.h>

#include <vector>

namespace simp
{

class Allocator;

class StaticAlloc
{
public:
	Allocator* Create();
	void Release(Allocator*);

private:
	std::vector<Allocator*> m_freelist;

	SINGLETON_DECLARATION(StaticAlloc);

}; // StaticAlloc

}

#endif // _SPRITE_IMPORT_STATIC_ALLOC_H_