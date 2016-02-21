#ifndef _EASYRESPACKER_ANIM_TO_LUA_STRING_H_
#define _EASYRESPACKER_ANIM_TO_LUA_STRING_H_

#include "PackAnimation.h"

namespace erespacker
{

class AnimToLuaString
{
public:
	static void Pack(const PackAnimation* anim, ebuilder::CodeGenerator& gen);

private:
	static void PackFrame(const PackAnimation::Frame& frame, ebuilder::CodeGenerator& gen);

}; // AnimToLuaString

}

#endif // _EASYRESPACKER_ANIM_TO_LUA_STRING_H_