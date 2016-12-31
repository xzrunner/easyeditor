#include "ClipboxFromBin.h"
#include "pack_unpack.h"

#include "spritepack.h"

namespace erespacker
{

int ClipboxFromBin::Size()
{
	return SIZEOF_PANNEL;
}

void ClipboxFromBin::Unpack(uint8_t** ptr, PackClipbox* cb)
{
	cb->x = 0;
	cb->y = 0;

	uint32_t w;
	unpack(w, ptr);
	cb->w = w;

	uint32_t h;
	unpack(h, ptr);
	cb->h = h;

	uint32_t scissor;
	unpack(scissor, ptr);
}

}