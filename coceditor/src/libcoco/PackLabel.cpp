#include "PackLabel.h"
#include "tools.h"

#include <easybuilder.h>
#include <epbin.h>

namespace lua = ebuilder::lua;

namespace libcoco
{

PackLabel::PackLabel(int id)
	: IPackNode(id)
{
}

void PackLabel::PackToLuaString(ebuilder::CodeGenerator& gen, const d2d::TexturePacker& tp) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + m_filepath);

	lua::assign_with_end(gen, "type", "\"label\"");
	lua::assign_with_end(gen, "id", d2d::StringTools::ToString(m_id));

	std::ostringstream ss;
	ss << "font = \"" << font << "\", ";
	ss << "color = " << d2d::transColor(color, d2d::PT_BGRA) << ", ";
	ss << "align = " << (align_hori | (align_vert << 4)) << ", ";
	ss << "size = " << size << ", ";
	ss << "width = " << width << ", ";
	ss << "height = " << height << ", ";
	ss << "noedge = ";
	if (has_edge) {
		ss << "false";
	} else {
		ss << "true";
	}

	gen.line(ss.str());

	gen.detab();
	gen.line("},");
}

void PackLabel::UnpackFromLua(lua_State* L, const std::vector<d2d::Image*>& images)
{
	font = epbin::LuaDataHelper::GetStringField(L, "font");
	color = d2d::transColor((uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "color"), d2d::PT_ABGR);
	int align = epbin::LuaDataHelper::GetIntField(L, "align");
	align_hori = (d2d::HoriAlignType)(align & 0xff);
	align_vert = (d2d::VertAlignType)((align & 0xff00) >> 4);
	size = epbin::LuaDataHelper::GetIntField(L, "size");
	width = epbin::LuaDataHelper::GetIntField(L, "width");
	height = epbin::LuaDataHelper::GetIntField(L, "height");
	has_edge = epbin::LuaDataHelper::GetBoolField(L, "noedge");
}

int PackLabel::SizeOfPackToBin() const
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

void PackLabel::PackToBin(uint8_t** ptr, const d2d::TexturePacker& tp) const
{
	uint8_t _align = (align_hori | (align_vert << 4));
	pack2mem(_align, ptr);

	uint32_t _color = d2d::trans_color2int(color, d2d::PT_BGRA);
	pack2mem(_color, ptr);

	uint16_t _size = size;
	pack2mem(_size, ptr);

	uint16_t _width = width, _height = height;
	pack2mem(_width, ptr);
	pack2mem(_height, ptr);

	uint8_t _edge = has_edge ? 1 : 0;
	pack2mem(_edge, ptr);

	// todo
	uint8_t _space_h = 0, _space_w = 0;
	pack2mem(_space_h, ptr);
	pack2mem(_space_w, ptr);
	uint8_t _auto_scale = 0;
	pack2mem(_auto_scale, ptr);
}

}