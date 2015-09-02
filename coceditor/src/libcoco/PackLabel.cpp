#include "PackLabel.h"

#include <easybuilder.h>
#include <epbin.h>

namespace lua = ebuilder::lua;

namespace libcoco
{

PackLabel::PackLabel(int id)
	: IPackNode(id)
{
}

void PackLabel::PackToLuaString(ebuilder::CodeGenerator& gen,
						 const TexturePacker& tp) const
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

}