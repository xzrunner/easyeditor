#include "LabelToBin.h"
#include "tools.h"
#include "spritepack.h"

namespace libcoco
{

int LabelToBin::Size()
{
	int sz = 0;
	sz += sizeof(uint16_t);		// id
	sz += sizeof(uint8_t);		// type
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
	uint16_t id = label->GetID();
	pack(id, ptr);

	uint8_t type = TYPE_LABEL;
	pack(type, ptr);

	uint8_t align = (label->align_hori | (label->align_vert << 4));
	pack(align, ptr);

	uint32_t color = d2d::trans_color2int(label->color, d2d::PT_BGRA);
	pack(color, ptr);

	uint16_t size = label->size;
	pack(size, ptr);

	uint16_t width = label->width, height = label->height;
	pack(width, ptr);
	pack(height, ptr);

	uint8_t edge = label->has_edge ? 1 : 0;
	pack(edge, ptr);

	// todo
	uint8_t space_h = 0, space_w = 0;
	pack(space_h, ptr);
	pack(space_w, ptr);
	uint8_t auto_scale = 0;
	pack(auto_scale, ptr);
}

}