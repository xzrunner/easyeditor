#ifndef _LIBCOMPLEX_PICTURE_TO_SPR_H_
#define _LIBCOMPLEX_PICTURE_TO_SPR_H_


#include <easyrespacker.h>

namespace ecomplex
{

class PictureToSpr
{
public:
	static ee::Sprite* Trans(const librespacker::PackPicture* pic);

private:
	static ee::Sprite* TransQuad(const librespacker::PackPicture::Quad* quad);

}; // PictureToSpr

}

#endif // _LIBCOMPLEX_PICTURE_TO_SPR_H_