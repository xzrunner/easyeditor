#ifndef _EASYRESPACKER_TRAIL_TO_BIN_H_
#define _EASYRESPACKER_TRAIL_TO_BIN_H_

#include "NodeToBin.h"
#include "PackTrail.h"

namespace erespacker
{

class TrailToBin : private NodeToBin
{
public:
	static int Size(const PackTrail* mask);

	static void Pack(const PackTrail* mask, uint8_t** ptr);

private:
	static int CompImageSize();
	static void PackCompImage(const PackTrail::CompImage& comp, uint8_t** ptr);

	static int CompShapeSize();
	static void PackCompShape(const PackTrail::CompShape& comp, uint8_t** ptr);

}; // TrailToBin

}

#endif // _EASYRESPACKER_TRAIL_TO_BIN_H_