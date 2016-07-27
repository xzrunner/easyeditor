#include "LabelFromLua.h"
#include "LuaDataHelper.h"

namespace erespacker
{

void LabelFromLua::Unpack(lua_State* L, PackLabel* label)
{
	label->width = LuaDataHelper::GetIntField(L, "width");
	label->height = LuaDataHelper::GetIntField(L, "height");

	label->font = LuaDataHelper::GetIntField(L, "font");
	label->font_size = LuaDataHelper::GetIntField(L, "font_size");
	label->font_color.FromRGBA((uint32_t)LuaDataHelper::GetDoubleField(L, "font_color"));

	label->edge = LuaDataHelper::GetBoolField(L, "edge");
	label->edge_size = static_cast<float>(LuaDataHelper::GetIntField(L, "edge_size"));
	label->edge_color.FromRGBA((uint32_t)LuaDataHelper::GetDoubleField(L, "edge_color"));

	label->align_hori = LuaDataHelper::GetIntField(L, "align_hori");
	label->align_vert = LuaDataHelper::GetIntField(L, "align_vert");

	label->space_hori = static_cast<float>(LuaDataHelper::GetDoubleField(L, "space_hori"));
	label->space_vert = static_cast<float>(LuaDataHelper::GetDoubleField(L, "space_vert"));

	label->overflow = LuaDataHelper::GetBoolField(L, "overflow");

	label->richtext = LuaDataHelper::GetBoolField(L, "richtext");

	label->text = LuaDataHelper::GetStringField(L, "text");
	label->tid = LuaDataHelper::GetStringField(L, "tid");
}

}