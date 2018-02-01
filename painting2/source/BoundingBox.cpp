#include "painting2/BoundingBox.h"

#include <memmgr/Allocator.h>

namespace pt2
{

void BoundingBox::Deleter(BoundingBox* bb)
{
	size_t sz = bb->Size();
	bb->~BoundingBox();
	mm::AllocHelper::Free(bb, sz);
};

}