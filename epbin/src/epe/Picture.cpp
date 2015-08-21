#include "Picture.h"
#include "type.h"

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

Picture::Picture(lua_State* L)
	: INode(TYPE_PICTURE)
{
	Load(L);
}

size_t Picture::Size() const
{
	size_t sz = INode::Size();
	sz += sizeof(uint16_t);		// size
	sz += sizeof(Quad) * m_quads.size();
	return sz;
}

void Picture::Store(uint8_t** ptr)
{
	INode::Store(ptr);

	uint16_t sz = m_quads.size();
	memcpy(*ptr, &sz, sizeof(sz));
	*ptr += sizeof(sz);

	for (int i = 0, n = m_quads.size(); i < n; ++i) {
		const Quad& q = m_quads[i];
		
		memcpy(*ptr, &q.texid, sizeof(q.texid));
		*ptr += sizeof(q.texid);

		for (int i = 0; i < 8; ++i) {
			memcpy(*ptr, &q.texture_coord[i], sizeof(q.texture_coord[i]));
			*ptr += sizeof(q.texture_coord[i]);
		}

		for (int i = 0; i < 8; ++i) {
			memcpy(*ptr, &q.screen_coord[i], sizeof(q.screen_coord[i]));
			*ptr += sizeof(q.screen_coord[i]);
		}
	}
}

void Picture::Load(lua_State* L)
{
	int len = lua_rawlen(L, -1);
	m_quads.reserve(len);
	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		Picture::Quad quad;
		// texid
		lua_getfield(L, -1, "tex");
		const char* type = lua_typename(L, lua_type(L, -1));
		quad.texid = (uint8_t)lua_tointeger(L, -1);
		lua_pop(L, 1);
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