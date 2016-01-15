#ifndef _LIBRESPACKER_P3D_SPR_FROM_BIN_H_
#define _LIBRESPACKER_P3D_SPR_FROM_BIN_H_

#include "NodeFromBin.h"

#include <stdint.h>

namespace librespacker
{

class PackP3dSpr;

class P3dSprFromBin : private NodeFromBin
{
public:
	static int Size();

	static void Unpack(uint8_t** ptr, PackP3dSpr* spr);

}; // P3dSprFromBin

}

#endif // _LIBRESPACKER_P3D_SPR_FROM_BIN_H_