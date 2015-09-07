#include "PictureFromBin.h"
#include "tools.h"
#include "spritepack.h"

namespace libcoco
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
		UnpackQuad(ptr, images, pic->quads[i]);
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
		quad.screen_coord[i].x = x;
		quad.screen_coord[i].y = y;
	}
}

}