#ifndef _LIBRESPACKER_PACK_TO_LUA_STRING_H_
#define _LIBRESPACKER_PACK_TO_LUA_STRING_H_

#include <drag2d.h>

namespace librespacker
{

class PackToLuaString
{
public:
	static void Pack(const std::string& filepath, const d2d::TexturePacker& tp,
		float scale);

}; // PackToLuaString

}

#endif // _LIBRESPACKER_PACK_TO_LUA_STRING_H_