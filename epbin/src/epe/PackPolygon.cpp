#include "PackPolygon.h"
#include "spritepack.h"

#include "../LuaDataHelper.h"
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

PackPolygon::PackPolygon(lua_State* L, uint32_t id)
	: IPackNode(TYPE_POLYGON, id)
{
	Load(L);
}

size_t PackPolygon::Size() const
{
	size_t sz = 0;

	sz += sizeof(uint8_t);

	int point_size = 0;
	for (int i = 0, n = m_polys.size(); i < n; ++i) {
		point_size += m_polys[i].texture_coord.size();
	}
	sz += SIZEOF_POLYGON 
		+ m_polys.size() * SIZEOF_POLYGON
		+ 12 * point_size;

	return sz;
}

void PackPolygon::Store(std::ofstream& fout) const
{
	uint8_t sz = m_polys.size();
	pack2file(sz, fout);
	for (int i = 0; i < sz; ++i) 
	{
		const Poly& poly = m_polys[i];

		pack2file(poly.texid, fout);

		uint8_t sz = poly.texture_coord.size() / 2;
		pack2file(sz, fout);
		for (int j = 0, m = poly.texture_coord.size(); j < m; ++j) {
			pack2file(poly.texture_coord[j], fout);
		}
		for (int j = 0, m = poly.screen_coord.size(); j < m; ++j) {
			pack2file(poly.screen_coord[j], fout);
		}
	}
}

void PackPolygon::Load(lua_State* L)
{
	int len = lua_rawlen(L, -1);
	m_polys.reserve(len);
	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		Poly poly;

		// tex
		if (LuaDataHelper::HasField(L, "tex")) {
			poly.texid = 0;
		} else {
			poly.texid = LuaDataHelper::GetIntField(L, "tex");
		}
		if (poly.texid > m_maxid) {
			m_maxid = poly.texid;
		}
		
		// src
		lua_getfield(L, -1, "src");
		assert(lua_type(L, -1) != LUA_TNIL);
		int len_s = lua_rawlen(L, -1);
		for (int i = 1; i <= len_s; ++i) {
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			poly.texture_coord.push_back(lua_tointeger(L, -1));
			lua_pop(L, 1);
		}
		lua_pop(L, 1);

		// screen
		lua_getfield(L, -1, "screen");
		assert(lua_type(L, -1) != LUA_TNIL);
		int len_d = lua_rawlen(L, -1);
		for (int i = 1; i <= len_d; ++i) {
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			poly.screen_coord.push_back(lua_tointeger(L, -1));
			lua_pop(L, 1);
		}
		lua_pop(L, 1);

		assert(len_s == len_d);

		lua_pop(L, 1);
	}
}

}
}