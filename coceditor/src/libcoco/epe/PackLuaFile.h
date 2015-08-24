#ifndef _COCO_PACK_LUA_FILE_H_
#define _COCO_PACK_LUA_FILE_H_

#include <drag2d.h>

namespace libcoco
{
namespace epe
{

class PackLuaFile
{
public:
	void pack(const std::vector<const d2d::ISprite*>& sprites, 
		const std::string& outfloder);

private:
	

}; // PackLuaFile

}
}

#endif // _COCO_PACK_LUA_FILE_H_