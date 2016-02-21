#include "PackLabel.h"
#include "spritepack.h"

#include "../LuaDataHelper.h"
#include "../tools.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

namespace epbin
{
namespace epe
{

PackLabel::PackLabel(lua_State* L, uint32_t id)
	: IPackNode(TYPE_LABEL, id)
{
	Load(L);
}

size_t PackLabel::Size() const
{
	return SIZEOF_LABEL;
}

void PackLabel::Store(std::ofstream& fout) const
{
	pack2file(m_label.align, fout);
	pack2file(m_label.color, fout);
	pack2file(m_label.size, fout);
	pack2file(m_label.width, fout);
	pack2file(m_label.height, fout);
	pack2file(m_label.edge, fout);
	pack2file(m_label.space_h, fout);
	pack2file(m_label.space_w, fout);
	pack2file(m_label.auto_scale, fout);
}

void PackLabel::Load(lua_State* L)
{
	memset(&m_label, 0, sizeof(Label));

	m_label.align = LuaDataHelper::GetIntField(L, "align");
	m_label.color = (uint32_t)LuaDataHelper::GetDoubleField(L, "color");
	m_label.size = LuaDataHelper::GetIntField(L, "size");
	m_label.width = LuaDataHelper::GetIntField(L, "width");
	m_label.height = LuaDataHelper::GetIntField(L, "height");
	if (LuaDataHelper::HasField(L, "noedge")) {
		int noedge = LuaDataHelper::GetIntField(L, "noedge");
		m_label.edge = noedge ? 0 : 1;
	}
	if (LuaDataHelper::HasField(L, "space_w")) {
		m_label.space_w = LuaDataHelper::GetIntField(L, "space_w");
	}
	if (LuaDataHelper::HasField(L, "space_h")) {
		m_label.space_h = LuaDataHelper::GetIntField(L, "space_h");
	}
	if (LuaDataHelper::HasField(L, "auto_size")) {
		m_label.auto_scale = LuaDataHelper::GetIntField(L, "auto_size");
	}
}

}
}