#ifndef _LIBRESPACKER_PICTURE_TO_BIN_H_
#define _LIBRESPACKER_PICTURE_TO_BIN_H_

#include "PackPicture.h"

namespace librespacker
{

class PictureToBin
{
public:
	static int Size(const PackPicture* pic);

	static void Pack(const PackPicture* pic, uint8_t** ptr,
		const ee::TexturePacker& tp, float scale);

private:
	static int SizeQuad();

	static void PackQuad(const PackPicture::Quad& quad, uint8_t** ptr, 
		const ee::TexturePacker& tp, float scale);

}; // PictureToBin

}

#endif // _LIBRESPACKER_PICTURE_TO_BIN_H_