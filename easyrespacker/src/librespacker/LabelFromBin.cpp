#include "LabelFromBin.h"
#include "pack_unpack.h"

#include <spritepack.h>

namespace librespacker
{

int LabelFromBin::Size(const PackLabel* label)
{
	int sz = SIZEOF_LABEL;

	sz += sizeof_unpack_str(label->text);	
	sz += sizeof_unpack_str(label->tid);	

	return sz;
}

void LabelFromBin::Unpack(uint8_t** ptr, PackLabel* label)
{
	uint16_t w, h;
	unpack(w, ptr);
	unpack(h, ptr);
	label->width = w;
	label->height = h;

	uint8_t font;
	unpack(font, ptr);
	label->font = font;

	uint8_t font_size;
	unpack(font_size, ptr);
	label->font_size = font_size;

	uint32_t font_color;
	unpack(font_color, ptr);
	label->font_color = ee::TransColor(font_color, ee::PT_RGBA);

	uint8_t edge;
	unpack(edge, ptr);
	label->edge = edge == 0 ? false : true;

	uint16_t edge_size;
	unpack(edge_size, ptr);
	label->edge_size = edge_size / 1024.0f;

	uint32_t edge_color;
	unpack(edge_color, ptr);
	label->edge_color = ee::TransColor(edge_color, ee::PT_RGBA);

	uint8_t align_hori, align_vert;
	unpack(align_hori, ptr);
	unpack(align_vert, ptr);
	label->align_hori = align_hori;
	label->align_vert = align_vert;

	uint16_t space_hori, space_vert;
	unpack(space_hori, ptr);
	unpack(space_vert, ptr);
	label->space_hori = space_hori / 1024.0f;
	label->space_vert = space_vert / 1024.0f;

	unpack_str(label->text, ptr);
	unpack_str(label->tid, ptr);
}

}