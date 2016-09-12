#ifndef _EASYSPRPACKER_PACK_TO_LUA_H_
#define _EASYSPRPACKER_PACK_TO_LUA_H_

#include <string>
#include <vector>

namespace ee { class TexturePacker; }

namespace esprpacker
{

class PackToLua
{
public:
	static void Pack(const std::string& filepath, const ee::TexturePacker& tp,
		float scale);

}; // PackToLua

}

#endif // _EASYSPRPACKER_PACK_TO_LUA_H_