#include "LabelFromBin.h"
#include "spritepack.h"
#include "tools.h"

namespace librespacker
{

int LabelFromBin::Size()
{
	return SIZEOF_LABEL;
}

void LabelFromBin::Unpack(uint8_t** ptr, PackLabel* label)
{
	uint8_t align;
	unpack(align, ptr);
	label->align_hori = (d2d::HoriAlignType)(align & 0xff);
	label->align_vert = (d2d::VertAlignType)((align & 0xff00) >> 4);

	uint32_t color;
	unpack(color, ptr);
	label->color = d2d::transColor(color, d2d::PT_BGRA);

	uint16_t size;
	unpack(size, ptr);
	label->size = size;

	uint16_t width, height;
	unpack(width, ptr);
	unpack(height, ptr);
	label->width = width;
	label->height = height;

	uint8_t edge;
	unpack(edge, ptr);
	label->has_edge = edge == 0 ? false : true;

	// todo
	uint8_t space_h, space_w;
	unpack(space_h, ptr);
	unpack(space_w, ptr);
	uint8_t auto_scale;
	unpack(auto_scale, ptr);
}

}