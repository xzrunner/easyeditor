#ifndef _EASYRESPACKER_CLIPBOX_TO_LUA_STRING_H_
#define _EASYRESPACKER_CLIPBOX_TO_LUA_STRING_H_

#include "PackClipbox.h"

namespace erespacker
{

class ClipboxToLuaString
{
public:
	static void Pack(const PackClipbox* cb, ebuilder::CodeGenerator& gen);

}; // ClipboxToLuaString

}

#endif // _EASYRESPACKER_CLIPBOX_TO_LUA_STRING_H_