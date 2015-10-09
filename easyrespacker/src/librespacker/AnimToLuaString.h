#ifndef _LIBRESPACKER_ANIM_TO_LUA_STRING_H_
#define _LIBRESPACKER_ANIM_TO_LUA_STRING_H_

#include "PackAnimation.h"

namespace librespacker
{

class AnimToLuaString
{
public:
	static void Pack(const PackAnimation* anim, ebuilder::CodeGenerator& gen);

private:
	static void PackFrame(const PackAnimation::Frame& frame, ebuilder::CodeGenerator& gen);

}; // AnimToLuaString

}

#endif // _LIBRESPACKER_ANIM_TO_LUA_STRING_H_