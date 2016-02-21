#include "PictureFromLua.h"
#include "typedef.h"

#include <assert.h>

namespace erespacker
{

void PictureFromLua::Unpack(lua_State* L, const std::vector<ee::Image*>& images, 
							PackPicture* pic)
{
	int len = lua_rawlen(L, -1);
	pic->quads.reserve(len);
	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		PackPicture::Quad quad;
		// tex
		lua_getfield(L, -1, "tex");
		const char* type = lua_typename(L, lua_type(L, -1));
		int tex_idx = (uint8_t)lua_tointeger(L, -1);
		assert(tex_idx < images.size());
		quad.img = images[tex_idx];
		lua_pop(L, 1);
		// src
		lua_getfield(L, -1, "src");
		int len = lua_rawlen(L, -1);
		assert(len == 8);
		for (int i = 1; i <= len; ++i)
		{
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			float src = static_cast<float>(lua_tonumber(L, -1));
			if (i % 2) {
				quad.texture_coord[(i - 1) / 2].x = src;
			} else {
				quad.texture_coord[(i - 1) / 2].y = src;
			}
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		// screen
		lua_getfield(L, -1, "screen");
		len = lua_rawlen(L, -1);
		assert(len == 8);
		for (int i = 1; i <= len; ++i)
		{
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			float screen = static_cast<float>(lua_tonumber(L, -1));
			if (i % 2) {
				quad.screen_coord[(i - 1) / 2].x = screen / SCALE;
			} else {
				quad.screen_coord[(i - 1) / 2].y = -screen / SCALE;
			}
			lua_pop(L, 1);
		}
		lua_pop(L, 1);

		pic->quads.push_back(quad);

		lua_pop(L, 1);
	}
}

}