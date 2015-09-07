#ifndef _LIBCOCO_ANIM_FROM_BIN_H_
#define _LIBCOCO_ANIM_FROM_BIN_H_

#include "PackAnimation.h"

namespace libcoco
{

class AnimFromBin
{
public:
	static int Size(const PackAnimation* anim);

	static void Unpack(uint8_t** ptr, PackAnimation* anim);

private:
	static int FrameSize(const PackAnimation::Frame& frame);

	static void UnpackComponents(uint8_t** ptr, PackAnimation* anim);
	static void UnpackActions(uint8_t** ptr, PackAnimation* anim);
	static void UnpackFrames(uint8_t** ptr, PackAnimation* anim);

	static void UnpackFrame(uint8_t** ptr, PackAnimation::Frame& frame);	
	static void UnpackPart(uint8_t** ptr, PackAnimation::Frame& frame);	

}; // AnimFromBin

}

#endif // _LIBCOCO_ANIM_FROM_BIN_H_