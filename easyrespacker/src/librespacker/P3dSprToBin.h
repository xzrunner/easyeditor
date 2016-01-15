#ifndef _LIBRESPACKER_P3D_SPR_TO_BIN_H_
#define _LIBRESPACKER_P3D_SPR_TO_BIN_H_

#include "NodeToBin.h"

#include <stdint.h>

namespace librespacker
{

class PackP3dSpr;

class P3dSprToBin : private NodeToBin
{
public:
	static int Size();

	static void Pack(const PackP3dSpr* spr, uint8_t** ptr);

}; // P3dSprToBin

}

#endif // _LIBRESPACKER_P3D_SPR_TO_BIN_H_