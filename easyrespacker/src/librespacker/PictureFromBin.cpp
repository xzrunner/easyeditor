#include "PictureFromBin.h"
#include "pack_unpack.h"
#include "typedef.h"

#include <spritepack.h>

namespace librespacker
{

int PictureFromBin::Size(const PackPicture* pic)
{
	return SIZEOF_PICTURE + pic->quads.size() * SIZEOF_QUAD;
}

void PictureFromBin::Unpack(uint8_t** ptr, const std::vector<d2d::Image*>& images,
							PackPicture* pic)
{
	uint16_t sz;
	unpack(sz, ptr);
	for (int i = 0; i < sz; ++i) {
		PackPicture::Quad quad;
		UnpackQuad(ptr, images, quad);
		pic->quads.push_back(quad);
	}
}

void PictureFromBin::UnpackQuad(uint8_t** ptr, const std::vector<d2d::Image*>& images,
								PackPicture::Quad& quad)
{
	uint8_t tex_idx;
	unpack(tex_idx, ptr);
	assert(tex_idx < images.size());
	quad.img = images[tex_idx];

	for (int i = 0; i < 4; ++i) {
		uint16_t x, y;
		unpack(x, ptr);
		unpack(y, ptr);
		quad.texture_coord[i].x = x;
		quad.texture_coord[i].y = y;
	}

	for (int i = 0; i < 4; ++i) {
		int32_t x, y;
		unpack(x, ptr);
		unpack(y, ptr);
		quad.screen_coord[i].x = (float)x / SCALE;
		quad.screen_coord[i].y = -(float)y / SCALE;
	}
}

}