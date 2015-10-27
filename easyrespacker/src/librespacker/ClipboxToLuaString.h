#ifndef _LIBRESPACKER_CLIPBOX_TO_LUA_STRING_H_
#define _LIBRESPACKER_CLIPBOX_TO_LUA_STRING_H_

#include "PackClipbox.h"

namespace librespacker
{

class ClipboxToLuaString
{
public:
	static void Pack(const PackClipbox* cb, ebuilder::CodeGenerator& gen);

}; // ClipboxToLuaString

}

#endif // _LIBRESPACKER_CLIPBOX_TO_LUA_STRING_H_