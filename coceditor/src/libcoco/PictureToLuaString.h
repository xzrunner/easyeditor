#ifndef _LIBCOCO_PICTURE_TO_LUA_STRING_H_
#define _LIBCOCO_PICTURE_TO_LUA_STRING_H_

#include "PackPicture.h"

namespace libcoco
{

class PictureToLuaString
{
public:
	static void Pack(const PackPicture* pic, ebuilder::CodeGenerator& gen,
		const d2d::TexturePacker& tp);

private:
	static void PackQuad(const PackPicture::Quad& quad, ebuilder::CodeGenerator& gen, 
		const d2d::TexturePacker& tp);

}; // PictureToLuaString

}

#endif // _LIBCOCO_PICTURE_TO_LUA_STRING_H_