#include "LabelToBin.h"
#include "tools.h"

namespace libcoco
{

int LabelToBin::Size()
{
	int sz = 0;
	sz += sizeof(uint8_t);		// align
	sz += sizeof(uint32_t);		// color
	sz += sizeof(uint16_t);		// size
	sz += sizeof(uint16_t) * 2;	// width, height
	sz += sizeof(uint8_t);		// edge
	sz += sizeof(uint8_t) * 2;	// space
	sz += sizeof(uint8_t);		// auto_scale
	return sz;
}

void LabelToBin::Pack(const PackLabel* label, uint8_t** ptr)
{
	uint8_t align = (label->align_hori | (label->align_vert << 4));
	pack2mem(align, ptr);

	uint32_t color = d2d::trans_color2int(label->color, d2d::PT_BGRA);
	pack2mem(color, ptr);

	uint16_t size = label->size;
	pack2mem(size, ptr);

	uint16_t width = label->width, height = label->height;
	pack2mem(width, ptr);
	pack2mem(height, ptr);

	uint8_t edge = label->has_edge ? 1 : 0;
	pack2mem(edge, ptr);

	// todo
	uint8_t space_h = 0, space_w = 0;
	pack2mem(space_h, ptr);
	pack2mem(space_w, ptr);
	uint8_t auto_scale = 0;
	pack2mem(auto_scale, ptr);
}

}