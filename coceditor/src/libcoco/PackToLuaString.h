#ifndef _LIBCOCO_PACK_TO_LUA_STRING_H_
#define _LIBCOCO_PACK_TO_LUA_STRING_H_

#include <drag2d.h>

namespace libcoco
{

class PackToLuaString
{
public:
	static void Pack(const std::string& filepath, const d2d::TexturePacker& tp);

}; // PackToLuaString

}

#endif // _LIBCOCO_PACK_TO_LUA_STRING_H_