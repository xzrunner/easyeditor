#include "LabelFromLua.h"

#include <epbin.h>

namespace librespacker
{

void LabelFromLua::Unpack(lua_State* L, PackLabel* label)
{
	label->font = epbin::LuaDataHelper::GetStringField(L, "font");
	label->color = d2d::transColor((uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "color"), d2d::PT_ABGR);
	int align = epbin::LuaDataHelper::GetIntField(L, "align");
	label->align_hori = (d2d::HoriAlignType)(align & 0xff);
	label->align_vert = (d2d::VertAlignType)((align & 0xff00) >> 4);
	label->size = epbin::LuaDataHelper::GetIntField(L, "size");
	label->width = epbin::LuaDataHelper::GetIntField(L, "width");
	label->height = epbin::LuaDataHelper::GetIntField(L, "height");
	label->has_edge = epbin::LuaDataHelper::GetBoolField(L, "noedge");
}

}