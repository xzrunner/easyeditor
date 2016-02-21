#ifndef _EASYRESPACKER_PICTURE_FROM_BIN_H_
#define _EASYRESPACKER_PICTURE_FROM_BIN_H_

#include "PackPicture.h"

namespace erespacker
{

class PictureFromBin
{
public:
	static int Size(const PackPicture* pic);

	static void Unpack(uint8_t** ptr, const std::vector<ee::Image*>& images,
		PackPicture* pic);

private:
	static void UnpackQuad(uint8_t** ptr, const std::vector<ee::Image*>& images,
		PackPicture::Quad& quad);

}; // PictureFromBin

}

#endif // _EASYRESPACKER_PICTURE_FROM_BIN_H_