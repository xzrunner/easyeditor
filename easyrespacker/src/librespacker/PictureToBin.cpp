#include "PictureToBin.h"
#include "pack_unpack.h"
#include "typedef.h"

#include <ee/TexturePacker.h>
#include <ee/Image.h>
#include <ee/ImageData.h>

#define TYPE_PICTURE		1

namespace erespacker
{

int PictureToBin::Size(const PackPicture* pic)
{
	int sz = 0;
	sz += sizeof(uint16_t);		// id
	sz += sizeof(uint8_t);		// type
	sz += sizeof(uint16_t);		// quads.size();
	sz += SizeQuad() * pic->quads.size();
	return sz;
}

void PictureToBin::Pack(const PackPicture* pic, uint8_t** ptr, 
						const ee::TexturePacker& tp, float scale)
{
	uint16_t id = pic->GetSprID();
	pack(id, ptr);

	uint8_t type = TYPE_PICTURE;
	pack(type, ptr);

	uint16_t sz = pic->quads.size();
	pack(sz, ptr);	
	for (int i = 0, n = pic->quads.size(); i < n; ++i) {
		PackQuad(pic->quads[i], ptr, tp, scale);
	}
}

int PictureToBin::SizeQuad()
{
	int ret = 0;
	ret += sizeof(uint8_t);		// texid
	ret += (sizeof(uint16_t) + sizeof(int32_t)) * 8;
	return ret;
}

void PictureToBin::PackQuad(const PackPicture::Quad& quad, uint8_t** ptr,
							 const ee::TexturePacker& tp, float scale)
{
	uint8_t idx = tp.QueryIdx(quad.img->GetFilepath());
	if (idx == 0xff) {
		idx = tp.QueryIdx(ee::ImageDataMgr::Instance()->GetDefaultSym());
	}
	pack(idx, ptr);

	int src[8];
	PackPicture::GetImgSrcPos(tp, quad.img.get(), quad.texture_coord, src);
	for (int i = 0; i < 8; ++i) {
		uint16_t p = static_cast<int>(src[i] * scale);
		pack(p, ptr);
	}

	for (int i = 0; i < 4; ++i) {
		int32_t x = static_cast<int>(floor(quad.screen_coord[i].x * SCALE + 0.5f)),
			    y =-static_cast<int>(floor(quad.screen_coord[i].y * SCALE + 0.5f));
		pack(x, ptr);
		pack(y, ptr);
	}
}

}