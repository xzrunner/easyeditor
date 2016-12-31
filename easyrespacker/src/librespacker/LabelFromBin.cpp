#include "LabelFromBin.h"
#include "pack_unpack.h"
#include "PackLabel.h"

#include "spritepack.h"

namespace erespacker
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
	label->font_color.FromRGBA(font_color);

	uint8_t edge;
	unpack(edge, ptr);
	label->edge = TransBool(edge);

	uint16_t edge_size;
	unpack(edge_size, ptr);
	label->edge_size = TransFloatX1024(edge_size);

	uint32_t edge_color;
	unpack(edge_color, ptr);
	label->edge_color.FromRGBA(edge_color);

	uint8_t align_hori, align_vert;
	unpack(align_hori, ptr);
	unpack(align_vert, ptr);
	label->align_hori = align_hori;
	label->align_vert = align_vert;

	uint16_t space_hori, space_vert;
	unpack(space_hori, ptr);
	unpack(space_vert, ptr);
	label->space_hori = TransFloatX1024(space_hori);
	label->space_vert = TransFloatX1024(space_vert);

	uint8_t pack8;
	unpack(pack8, ptr);
	uint8_t richtext = pack8 & 0x1;
	uint8_t overflow = pack8 & 0x2;;
	label->richtext = TransBool(richtext);
	label->overflow = TransBool(overflow);

	unpack_str(label->text, ptr);
	unpack_str(label->tid, ptr);
}

}