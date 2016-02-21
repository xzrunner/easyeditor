#include "P3dSprToBin.h"
#include "PackP3dSpr.h"
#include "pack_unpack.h"

#include <spritepack.h>

namespace erespacker
{

int P3dSprToBin::Size()
{
	int sz = 0;
	sz += sizeof(uint16_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint16_t);			// p3d id
	sz += sizeof(uint8_t) * 4;		// loop, local, alone, reuse
	return sz;
}

void P3dSprToBin::Pack(const PackP3dSpr* spr, uint8_t** ptr)
{
	uint16_t id = spr->GetSprID();
	pack(id, ptr);

	uint8_t type = TYPE_P3D_SPR;
	pack(type, ptr);

	uint16_t p3d_id = spr->p3d->GetSprID();
	pack(p3d_id, ptr);

	uint8_t loop = TransBool(spr->loop);
	pack(loop, ptr);

	uint8_t local = TransBool(spr->local);
	pack(local, ptr);

	uint8_t alone = TransBool(spr->alone);
	pack(alone, ptr);

	uint8_t reuse = TransBool(spr->reuse);
	pack(reuse, ptr);
}

}