#include "PackLabel.h"
#include "binary_io.h"
#include "to_int.h"

#include <easytext.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeLabel.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackLabel::PackLabel(const etext::Sprite* spr)
{
	Init(spr);
}

PackLabel::~PackLabel()
{
}

void PackLabel::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"label\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::connect(gen, 2, 
		lua::assign("width", m_tb.width), 
		lua::assign("height", m_tb.height));

	lua::connect(gen, 3, 
		lua::assign("font", m_tb.font_type), 
		lua::assign("font_size", m_tb.font_size),
		lua::assign("font_color", m_tb.font_color.ToRGBA()));

	lua::connect(gen, 3, 
		lua::assign("edge", m_tb.has_edge), 
		lua::assign("edge_size", m_tb.edge_size),
		lua::assign("edge_color", m_tb.edge_color.ToRGBA()));

	lua::connect(gen, 2, 
		lua::assign("align_hori", m_tb.align_hori), 
		lua::assign("align_vert", m_tb.align_vert));

	lua::connect(gen, 2, 
		lua::assign("space_hori", m_tb.space_hori), 
		lua::assign("space_vert", m_tb.space_vert));

	lua::connect(gen, 1, 
		lua::assign("overflow", m_tb.overflow));

	lua::connect(gen, 1, 
		lua::assign("richtext", m_tb.richtext));

	lua::connect(gen, 2, 
		lua::assign("text", "\"" + m_text + "\""),
		lua::assign("tid", "\"" + m_tid + "\""));

	gen.detab();
	gen.line("},");
}

int PackLabel::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeLabel::Size();
	sz += sizeof_unpack_str(m_text);	
	sz += sizeof_unpack_str(m_tid);	
	return sz;
}

int PackLabel::SizeOfPackToBin() const
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

	sz += sizeof_pack_str(m_text);		// text
	sz += sizeof_pack_str(m_tid);		// tid

	return sz;
}

void PackLabel::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = TYPE_LABEL;
	pack(type, ptr);

	uint16_t w = m_tb.width, h = m_tb.height;
	pack(w, ptr);
	pack(h, ptr);

	uint8_t font = m_tb.font_type;
	pack(font, ptr);
	uint8_t font_size = m_tb.font_size;
	pack(font_size, ptr);
	uint32_t font_color = m_tb.font_color.ToRGBA();
	pack(font_color, ptr);

	uint8_t edge = bool2int(m_tb.has_edge);
	pack(edge, ptr);
	uint16_t edge_size = float1024x2int(m_tb.edge_size);
	pack(edge_size, ptr);
	uint32_t edge_color = m_tb.edge_color.ToRGBA();
	pack(edge_color, ptr);

	uint8_t align_hori = m_tb.align_hori;
	pack(align_hori, ptr);
	uint8_t align_vert = m_tb.align_vert;
	pack(align_vert, ptr);

	uint16_t space_hori = float1024x2int(m_tb.space_hori);
	uint16_t space_vert = float1024x2int(m_tb.space_vert);
	pack(space_hori, ptr);
	pack(space_vert, ptr);

	uint8_t overflow = bool2int(m_tb.overflow);
	uint8_t richtext = bool2int(m_tb.richtext);
	uint8_t pack8 = (richtext) | (overflow << 1);
	pack(pack8, ptr);

	pack_str(m_text, ptr);
	pack_str(m_tid, ptr);
}

void PackLabel::Init(const etext::Sprite* spr)
{
	m_tb = spr->GetTextbox();

	m_text = spr->GetText();
	m_tid = spr->GetTID();
}

}