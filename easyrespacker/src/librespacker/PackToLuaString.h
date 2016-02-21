#ifndef _EASYRESPACKER_PACK_TO_LUA_STRING_H_
#define _EASYRESPACKER_PACK_TO_LUA_STRING_H_

#include <string>

namespace ee { class TexturePacker; }

namespace erespacker
{

class PackToLuaString
{
public:
	static void Pack(const std::string& filepath, const ee::TexturePacker& tp,
		float scale);

}; // PackToLuaString

}

#endif // _EASYRESPACKER_PACK_TO_LUA_STRING_H_