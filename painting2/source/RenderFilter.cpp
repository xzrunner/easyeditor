#include "painting2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace pt2
{

void RenderFilter::Deleter(RenderFilter* filter)
{
	size_t sz = filter->Size();
	filter->~RenderFilter();
	mm::AllocHelper::Free(filter, sz);
}

}