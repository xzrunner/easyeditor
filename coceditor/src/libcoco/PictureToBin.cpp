#include "PictureToBin.h"
#include "tools.h"

namespace libcoco
{

int PictureToBin::Size(const PackPicture* pic)
{
	int sz = 0;

	// type

	// id

	sz += sizeof(uint16_t);		// quads.size();
	sz += SizeQuad() * pic->quads.size();

	return sz;
}

void PictureToBin::Pack(const PackPicture* pic, uint8_t** ptr, const d2d::TexturePacker& tp)
{
	// type

	// id

	uint16_t sz = pic->quads.size();
	pack(sz, ptr);	
	for (int i = 0, n = pic->quads.size(); i < n; ++i) {
		PackQuad(pic->quads[i], ptr, tp);
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
							 const d2d::TexturePacker& tp)
{
	uint8_t idx = tp.QueryIdx(quad.img->GetFilepath());
	pack(idx, ptr);

	for (int i = 0; i < 4; ++i) {
		uint16_t x = quad.texture_coord[i].x,
			y = quad.texture_coord[i].y;
		pack(x, ptr);
		pack(y, ptr);
	}

	for (int i = 0; i < 4; ++i) {
		int32_t x = quad.screen_coord[i].x,
			y = quad.screen_coord[i].y;
		pack(x, ptr);
		pack(y, ptr);
	}
}

}