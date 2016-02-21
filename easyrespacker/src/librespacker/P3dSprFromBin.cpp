#include "P3dSprFromBin.h"
#include "UnpackNodeFactory.h"
#include "pack_unpack.h"
#include "PackP3dSpr.h"

#include <spritepack.h>

namespace erespacker
{

int P3dSprFromBin::Size()
{
	return SIZEOF_P3D_SPR;
}

void P3dSprFromBin::Unpack(uint8_t** ptr, PackP3dSpr* spr)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	uint16_t id;
	unpack(id, ptr);
	spr->p3d = factory->Query(id);
	if (!spr->p3d) {
		factory->AddUnassigned(id, &spr->p3d);
	}

	uint8_t loop;
	unpack(loop, ptr);
	spr->loop = TransBool(loop);

	uint8_t local;
	unpack(local, ptr);
	spr->local = TransBool(local);

	uint8_t alone;
	unpack(alone, ptr);
	spr->alone = TransBool(alone);

	uint8_t reuse;
	unpack(reuse, ptr);
	spr->reuse = TransBool(reuse);
}

}