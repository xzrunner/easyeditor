#ifndef _EASYRESPACKER_UNPACKE_FROM_LUA_H_
#define _EASYRESPACKER_UNPACKE_FROM_LUA_H_

#include <vector>

namespace ee { class Image; }

namespace erespacker
{

class UnpackFromLua
{
public:
	static void Unpack(const std::string& filepath, 
		const std::vector<ee::Image*>& images);

}; // UnpackFromLua

}

#endif // _EASYRESPACKER_UNPACKE_FROM_LUA_H_