#include "PackPannel.h"
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

PackPannel::PackPannel(lua_State* L, uint32_t id)
	: IPackNode(TYPE_PANNEL, id)
{
	Load(L);
}

size_t PackPannel::Size() const
{
	return SIZEOF_PANNEL;
}

void PackPannel::Store(std::ofstream& fout) const
{
	pack2file(m_pannel.width, fout);
	pack2file(m_pannel.height, fout);
	pack2file(m_pannel.scissor, fout);
}

void PackPannel::Load(lua_State* L)
{
	memset(&m_pannel, 0, sizeof(m_pannel));

	m_pannel.width = LuaDataHelper::GetIntField(L, "width");
	m_pannel.height = LuaDataHelper::GetIntField(L, "height");
	if (LuaDataHelper::HasField(L, "scissor")) {
		m_pannel.scissor = LuaDataHelper::GetIntField(L, "scissor");
	}
}

}
}