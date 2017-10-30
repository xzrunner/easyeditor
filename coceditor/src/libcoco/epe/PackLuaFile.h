#ifndef _COCO_EPE_PACK_LUA_FILE_H_
#define _COCO_EPE_PACK_LUA_FILE_H_

#include <ee/Sprite.h>

#include <vector>

namespace ecoco
{
namespace epe
{

class PackLuaFile
{
public:
	void pack(const std::vector<ee::SprPtr>& sprs, 
		const std::string& outfloder);

private:
	

}; // PackLuaFile

}
}

#endif // _COCO_EPE_PACK_LUA_FILE_H_