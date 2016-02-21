#ifndef _EASYRESPACKER_P3D_SPR_TO_BIN_H_
#define _EASYRESPACKER_P3D_SPR_TO_BIN_H_

#include "NodeToBin.h"

#include <stdint.h>

namespace erespacker
{

class PackP3dSpr;

class P3dSprToBin : private NodeToBin
{
public:
	static int Size();

	static void Pack(const PackP3dSpr* spr, uint8_t** ptr);

}; // P3dSprToBin

}

#endif // _EASYRESPACKER_P3D_SPR_TO_BIN_H_