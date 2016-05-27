#ifndef _EASYRESPACKER_MASK_TO_BIN_H_
#define _EASYRESPACKER_MASK_TO_BIN_H_

#include "NodeToBin.h"

namespace erespacker
{

class PackMask;

class MaskToBin : private NodeToBin
{
public:
	static int Size(const PackMask* mask);

	static void Pack(const PackMask* mask, uint8_t** ptr);

}; // MaskToBin

}

#endif // _EASYRESPACKER_MASK_TO_BIN_H_