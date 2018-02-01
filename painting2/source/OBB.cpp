#include "painting2/OBB.h"

#include <memmgr/Allocator.h>

namespace pt2
{

BoundingBox* OBB::Clone() const
{
	void* ptr = mm::AllocHelper::Allocate(Size());
	return new (ptr) OBB(*this);
}

}