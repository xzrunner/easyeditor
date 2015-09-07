#ifndef _LIBCOCO_PICTURE_FROM_BIN_H_
#define _LIBCOCO_PICTURE_FROM_BIN_H_

#include "PackPicture.h"

namespace libcoco
{

class PictureFromBin
{
public:
	static void Unpack(uint8_t** ptr, const std::vector<d2d::Image*>& images,
		PackPicture* pic);

private:
	static void UnpackQuad(uint8_t** ptr, const std::vector<d2d::Image*>& images,
		PackPicture::Quad& quad);

}; // PictureFromBin

}

#endif // _LIBCOCO_PICTURE_FROM_BIN_H_