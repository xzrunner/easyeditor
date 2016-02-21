#ifndef _EASYCOMPLEX_PICTURE_TO_SPR_H_
#define _EASYCOMPLEX_PICTURE_TO_SPR_H_


#include <easyrespacker.h>

namespace ecomplex
{

class PictureToSpr
{
public:
	static ee::Sprite* Trans(const erespacker::PackPicture* pic);

private:
	static ee::Sprite* TransQuad(const erespacker::PackPicture::Quad* quad);

}; // PictureToSpr

}

#endif // _EASYCOMPLEX_PICTURE_TO_SPR_H_