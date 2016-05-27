#ifndef _EASYRESPACKER_MASK_FROM_BIN_H_
#define _EASYRESPACKER_MASK_FROM_BIN_H_

#include "NodeFromBin.h"

namespace erespacker
{

class PackMask;

class MaskFromBin : private NodeFromBin
{
public:
	static int Size();

	static void Unpack(uint8_t** ptr, PackMask* mask);

}; // MaskFromBin

}

#endif // _EASYRESPACKER_MASK_FROM_BIN_H_