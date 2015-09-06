#ifndef _LIBCOCO_ANIM_TO_LUA_STRING_H_
#define _LIBCOCO_ANIM_TO_LUA_STRING_H_

#include "PackAnimation.h"

namespace libcoco
{

class AnimToLuaString
{
public:
	static void Pack(const PackAnimation* anim, ebuilder::CodeGenerator& gen);

private:
	static void PackFrame(const PackAnimation::Frame& frame, ebuilder::CodeGenerator& gen);

}; // AnimToLuaString

}

#endif // _LIBCOCO_ANIM_TO_LUA_STRING_H_