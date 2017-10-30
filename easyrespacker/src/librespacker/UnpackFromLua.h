#ifndef _EASYRESPACKER_UNPACKE_FROM_LUA_H_
#define _EASYRESPACKER_UNPACKE_FROM_LUA_H_

#include <ee/Image.h>

#include <vector>

namespace erespacker
{

class UnpackFromLua
{
public:
	static void Unpack(const std::string& filepath, 
		const std::vector<ee::ImagePtr>& images);

}; // UnpackFromLua

}

#endif // _EASYRESPACKER_UNPACKE_FROM_LUA_H_