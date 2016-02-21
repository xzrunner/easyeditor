#include "PackPicture.h"
#include "type.h"
#include "spritepack.h"

#include "../tools.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

#include <assert.h>

namespace epbin
{
namespace epe
{

PackPicture::PackPicture(lua_State* L, uint32_t id)
	: IPackNode(TYPE_PICTURE, id)
{
	Load(L);
}

size_t PackPicture::Size() const
{
	return SIZEOF_PICTURE + m_quads.size() * SIZEOF_QUAD;
}

void PackPicture::Store(std::ofstream& fout) const
{
	uint16_t sz = m_quads.size();
	pack2file(sz, fout);

	for (int i = 0, n = m_quads.size(); i < n; ++i) {
		const Quad& q = m_quads[i];

		pack2file(q.texid, fout);
		for (int i = 0; i < 8; ++i) {
			pack2file(q.texture_coord[i], fout);
		}
		for (int i = 0; i < 8; ++i) {
			pack2file(q.screen_coord[i], fout);
		}
	}
}

void PackPicture::Load(lua_State* L)
{
	int len = lua_rawlen(L, -1);
	m_quads.reserve(len);
	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		PackPicture::Quad quad;

		// texid
		lua_getfield(L, -1, "tex");
		quad.texid = (uint8_t)lua_tointeger(L, -1);
		lua_pop(L, 1);
		if (quad.texid > m_maxid) {
			m_maxid = quad.texid;
		}

		// texture_coord
		lua_getfield(L, -1, "src");
		int len = lua_rawlen(L, -1);
		assert(len == 8);
		uint16_t* src = &quad.texture_coord[0];
		for (int i = 1; i <= len; ++i)
		{
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			src[i-1] = (uint16_t)lua_tonumber(L, -1);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);

		// screen_coord
		lua_getfield(L, -1, "screen");
		len = lua_rawlen(L, -1);
		assert(len == 8);
		int32_t* dst = &quad.screen_coord[0];
		for (int i = 1; i <= len; ++i)
		{
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			dst[i-1] = lua_tointeger(L, -1);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);

		m_quads.push_back(quad);

		lua_pop(L, 1);
	}
}

}
}