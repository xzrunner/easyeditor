#include "MaskFromBin.h"
#include "PackMask.h"
#include "pack_unpack.h"
#include "UnpackNodeFactory.h"

// for SIZEOF_MASK
#include "spritepack.h"

#include <limits>

namespace erespacker
{

int MaskFromBin::Size()
{
	return SIZEOF_MASK;
}

void MaskFromBin::Unpack(uint8_t** ptr, PackMask* mask)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	uint16_t base_id;
	unpack(base_id, ptr);
	mask->base = factory->Query(base_id);
	if (!mask->base) {
		factory->AddUnassigned(base_id, &mask->base);
	}

	uint16_t mask_id;
	unpack(mask_id, ptr);
	mask->mask = factory->Query(mask_id);
	if (!mask->mask) {
		factory->AddUnassigned(mask_id, &mask->mask);
	}
}

}