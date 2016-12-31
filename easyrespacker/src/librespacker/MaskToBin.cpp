#include "MaskToBin.h"
#include "PackMask.h"
#include "NodeToBin.h"
#include "pack_unpack.h"

// for TYPE_MASK
#include "spritepack.h"

#include <limits>

namespace erespacker
{

int MaskToBin::Size(const PackMask* mask)
{
	int sz = 0;
	sz += sizeof(uint16_t);						// id
	sz += sizeof(uint8_t);						// type
	sz += sizeof(uint16_t);						// base id
	sz += sizeof(uint16_t);						// mask id
	return sz;
}

void MaskToBin::Pack(const PackMask* mask, uint8_t** ptr)
{
	uint16_t id = mask->GetSprID();
	pack(id, ptr);

	uint8_t type = TYPE_MASK;
	pack(type, ptr);
 
	assert(mask->base);
	uint16_t base_id = mask->base->GetSprID();
	pack(base_id, ptr);

	assert(mask->base);
	uint16_t mask_id = mask->mask->GetSprID();
	pack(mask_id, ptr);
}

}