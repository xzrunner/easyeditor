#include "LabelFromLua.h"
#include "LuaDataHelper.h"

namespace librespacker
{

void LabelFromLua::Unpack(lua_State* L, PackLabel* label)
{
	label->font = LuaDataHelper::GetStringField(L, "font");
	label->color = d2d::transColor((uint32_t)LuaDataHelper::GetDoubleField(L, "color"), d2d::PT_ABGR);
	int align = LuaDataHelper::GetIntField(L, "align");
	label->align_hori = (d2d::HoriAlignType)(align & 0xff);
	label->align_vert = (d2d::VertAlignType)((align & 0xff00) >> 4);
	label->size = LuaDataHelper::GetIntField(L, "size");
	label->width = LuaDataHelper::GetIntField(L, "width");
	label->height = LuaDataHelper::GetIntField(L, "height");
	label->has_edge = LuaDataHelper::GetBoolField(L, "noedge");
}

}