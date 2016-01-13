#include "LabelToBin.h"
#include "pack_unpack.h"

#include <spritepack.h>

namespace librespacker
{

int LabelToBin::Size(const PackLabel* label)
{
	int sz = 0;

	sz += sizeof(uint16_t);				// id
	sz += sizeof(uint8_t);				// type

	sz += sizeof(uint16_t) * 2;			// width, height
	
	sz += sizeof(uint8_t);				// font
	sz += sizeof(uint8_t);				// font_size
	sz += sizeof(uint32_t);				// font_color

	sz += sizeof(uint8_t);				// edge
	sz += sizeof(uint16_t);				// edge_size
	sz += sizeof(uint32_t);				// edge_color

	sz += sizeof(uint8_t) * 2;			// align

	sz += sizeof(uint16_t) * 2;			// space

	sz += sizeof_pack_str(label->text);	// text
	sz += sizeof_pack_str(label->tid);	// tid

	return sz;
}

void LabelToBin::Pack(const PackLabel* label, uint8_t** ptr)
{
	uint16_t id = label->GetSprID();
	pack(id, ptr);

	uint8_t type = TYPE_LABEL;
	pack(type, ptr);

	uint16_t w = label->width, h = label->height;
	pack(w, ptr);
	pack(h, ptr);

	uint8_t font = label->font;
	pack(font, ptr);
	uint8_t font_size = label->font_size;
	pack(font_size, ptr);
	uint32_t font_color = d2d::trans_color2int(label->font_color, d2d::PT_RGBA);
	pack(font_color, ptr);

	uint8_t edge = label->edge ? 1 : 0;
	pack(edge, ptr);
	uint16_t edge_size = (int)(label->edge_size * 1024);
	pack(edge_size, ptr);
	uint32_t edge_color = d2d::trans_color2int(label->edge_color, d2d::PT_RGBA);
	pack(edge_color, ptr);

	uint8_t align_hori = label->align_hori;
	pack(align_hori, ptr);
	uint8_t align_vert = label->align_vert;
	pack(align_vert, ptr);

	uint16_t space_hori = (int)(label->space_hori * 1024);
	uint16_t space_vert = (int)(label->space_vert * 1024);
	pack(space_hori, ptr);
	pack(space_vert, ptr);

	pack_str(label->text, ptr);
	pack_str(label->tid, ptr);
}

}