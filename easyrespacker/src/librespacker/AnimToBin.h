#ifndef _EASYRESPACKER_ANIM_TO_BIN_H_
#define _EASYRESPACKER_ANIM_TO_BIN_H_

#include "PackAnimation.h"

namespace erespacker
{

class AnimToBin
{
public:
	static int Size(const PackAnimation* anim);

	static void Pack(const PackAnimation* anim, uint8_t** ptr);

private:
	static int FrameSize(const PackAnimation::Frame& frame);

	static void PackFrame(const PackAnimation::Frame& frame, uint8_t** ptr);

}; // AnimToBin

}

#endif // _EASYRESPACKER_ANIM_TO_BIN_H_