#include "LabelToBin.h"
#include "pack_unpack.h"
#include "PackLabel.h"

#include "spritepack.h"

namespace erespacker
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

	sz += sizeof(uint8_t);				// richtext & overflow

	//sz += sizeof_pack_str(label->text);	// text
	//sz += sizeof_pack_str(label->tid);	// tid
	sz += sizeof_pack_str("");	// text
	sz += sizeof_pack_str("");	// tid

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
	uint32_t font_color = label->font_color.ToRGBA();
	pack(font_color, ptr);

	uint8_t edge = TransBool(label->edge);
	pack(edge, ptr);
	uint16_t edge_size = TransFloatX1024(label->edge_size);
	pack(edge_size, ptr);
	uint32_t edge_color = label->edge_color.ToRGBA();
	pack(edge_color, ptr);

	uint8_t align_hori = label->align_hori;
	pack(align_hori, ptr);
	uint8_t align_vert = label->align_vert;
	pack(align_vert, ptr);

	uint16_t space_hori = TransFloatX1024(label->space_hori);
	uint16_t space_vert = TransFloatX1024(label->space_vert);
	pack(space_hori, ptr);
	pack(space_vert, ptr);

	uint8_t overflow = TransBool(label->overflow);
	uint8_t richtext = TransBool(label->richtext);
	uint8_t pack8 = (richtext) | (overflow << 1);
	pack(pack8, ptr);

// 	pack_str(label->text, ptr);
// 	pack_str(label->tid, ptr);
	pack_str("", ptr);
	pack_str("", ptr);
}

}