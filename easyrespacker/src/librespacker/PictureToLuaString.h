#ifndef _LIBRESPACKER_PICTURE_TO_LUA_STRING_H_
#define _LIBRESPACKER_PICTURE_TO_LUA_STRING_H_

#include "PackPicture.h"

namespace librespacker
{

class PictureToLuaString
{
public:
	static void Pack(const PackPicture* pic, ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale);

private:
	static void PackQuad(const PackPicture::Quad& quad, ebuilder::CodeGenerator& gen, 
		const ee::TexturePacker& tp, float scale);

}; // PictureToLuaString

}

#endif // _LIBRESPACKER_PICTURE_TO_LUA_STRING_H_