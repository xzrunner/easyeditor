#include "ClipboxToBin.h"
#include "pack_unpack.h"

#include <spritepack.h>

namespace erespacker
{

int ClipboxToBin::Size()
{
	int sz = 0;
	sz += sizeof(uint16_t);		// id
	sz += sizeof(uint8_t);		// type
	sz += sizeof(uint32_t);		// width
	sz += sizeof(uint32_t);		// height
	sz += sizeof(uint8_t);		// scissor
	return sz;
}

void ClipboxToBin::Pack(const PackClipbox* cb, uint8_t** ptr)
{
	uint16_t id = cb->GetSprID();
	pack(id, ptr);

	uint8_t type = TYPE_PANNEL;
	pack(type, ptr);

	uint32_t w = cb->w;
	pack(w, ptr);

	uint32_t h = cb->h;
	pack(h, ptr);

	uint8_t scissor = 1;
	pack(scissor, ptr);
}

}