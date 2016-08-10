#ifndef _EASYRESPACKER_TRAIL_FROM_BIN_H_
#define _EASYRESPACKER_TRAIL_FROM_BIN_H_

#include "NodeFromBin.h"
#include "PackTrail.h"

namespace erespacker
{

class TrailFromBin : private NodeFromBin
{
public:
	static int Size(const PackTrail* trail);

	static void Unpack(uint8_t** ptr, PackTrail* trail);

}; // TrailFromBin

}

#endif // _EASYRESPACKER_TRAIL_FROM_BIN_H_