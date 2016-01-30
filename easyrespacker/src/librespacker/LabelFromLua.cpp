#include "LabelFromLua.h"
#include "LuaDataHelper.h"

namespace librespacker
{

void LabelFromLua::Unpack(lua_State* L, PackLabel* label)
{
	label->width = LuaDataHelper::GetIntField(L, "width");
	label->height = LuaDataHelper::GetIntField(L, "height");

	label->font = LuaDataHelper::GetIntField(L, "font");
	label->font_size = LuaDataHelper::GetIntField(L, "font_size");
	label->font_color = d2d::TransColor((uint32_t)LuaDataHelper::GetDoubleField(L, "font_color"), d2d::PT_RGBA);

	label->edge = LuaDataHelper::GetBoolField(L, "edge");
	label->edge_size = LuaDataHelper::GetIntField(L, "edge_size");
	label->edge_color = d2d::TransColor((uint32_t)LuaDataHelper::GetDoubleField(L, "edge_color"), d2d::PT_RGBA);

	label->align_hori = LuaDataHelper::GetIntField(L, "align_hori");
	label->align_vert = LuaDataHelper::GetIntField(L, "align_vert");

	label->space_hori = LuaDataHelper::GetDoubleField(L, "space_hori");
	label->space_vert = LuaDataHelper::GetDoubleField(L, "space_vert");

	label->text = LuaDataHelper::GetStringField(L, "text");
	label->tid = LuaDataHelper::GetStringField(L, "tid");
}

}