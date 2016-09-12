#include "NodeComplex.h"
#include "ImportStream.h"
#include "simp_define.h"
#include "Allocator.h"

namespace simp
{

NodeComplex::NodeComplex(Allocator& alloc, ImportStream& is)
{
	// scissor
	for (int i = 0; i < 4; ++i) {
		uint16_t s = is.UInt16();
		scissor[i] = s;
	}

	// actions
	actions_n = is.UInt16();
	uint8_t* actions_ptr = static_cast<uint8_t*>(alloc.Alloc(ActionSize() * actions_n));
	for (int i = 0; i < actions_n; ++i) 
	{
		Action* dst = (Action*)(actions_ptr + ActionSize() * i);
		dst->name = is.String(alloc);
		dst->n = is.UInt16();
		dst->idx = (uint16_t*)(alloc.Alloc(sizeof(uint16_t) * dst->n));
		for (int j = 0; j < dst->n; ++j) {
			dst->idx[j] = is.UInt16();
		}
	}

	// sprs
	sprs_n = is.UInt16();
	sprs = (uint32_t*)(alloc.Alloc(sizeof(int32_t) * sprs_n));
	for (int i = 0; i < sprs_n; ++i) {
		sprs[i] = is.UInt32();
	}
}

int NodeComplex::Size()
{
	return sizeof(NodeComplex) + PTR_SIZE_DIFF - ActionSize();
}

int NodeComplex::ActionSize()
{
	return sizeof(Action) + PTR_SIZE_DIFF * 2;
}

}