#include "PackPicture.h"
#include "TexturePacker.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace libcoco
{

static const float SCALE = 16;

void PackPicture::PackToLuaString(ebuilder::CodeGenerator& gen,
						   const TexturePacker& tp) const
{
	gen.line("{");
	gen.tab();

	lua::assign_with_end(gen, "type", "\"picture\"");
	lua::assign_with_end(gen, "id", d2d::StringTools::ToString(m_id));

	for (int i = 0, n = quads.size(); i < n; ++i) {
		QuadToString(quads[i], gen, tp);
	}

	gen.detab();
	gen.line("},");
}

void PackPicture::UnpackFromLua(lua_State* L, const std::vector<d2d::Image*>& images)
{
	int len = lua_rawlen(L, -1);
	quads.reserve(len);
	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		Quad quad;
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
			int src = lua_tonumber(L, -1);
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
			int screen = lua_tonumber(L, -1);
			if (i % 2) {
				quad.screen_coord[(i - 1) / 2].x = screen;
			} else {
				quad.screen_coord[(i - 1) / 2].y = screen;
			}
			lua_pop(L, 1);
		}
		lua_pop(L, 1);

		quads.push_back(quad);

		lua_pop(L, 1);
	}
}

void PackPicture::QuadToString(const Quad& quad, ebuilder::CodeGenerator& gen, 
							   const TexturePacker& tp)
{
	std::string tex_str = lua::assign("tex", d2d::StringTools::ToString(0));

	char buff[256];

	int src[8];
	GetImgSrcPos(tp, quad.img, src);
	sprintf(buff, "src = { %d, %d, %d, %d, %d, %d, %d, %d }", 
		src[0], src[1], src[2], src[3], src[4], src[5], src[6], src[7]);
	std::string src_str = buff;

	int screen[8];
	for (int j = 0; j < 4; ++j) {
		screen[j*2] = floor(quad.screen_coord[j].x * SCALE + 0.5f);
		screen[j*2+1] = -floor(quad.screen_coord[j].y * SCALE + 0.5f);
	}
	sprintf(buff, "screen = { %d, %d, %d, %d, %d, %d, %d, %d }", 
		screen[0], screen[1], screen[2], screen[3], screen[4], screen[5], screen[6], screen[7]);
	std::string screen_str = buff;

	lua::tableassign(gen, "", 3, tex_str.c_str(), src_str.c_str(), screen_str.c_str());
}

void PackPicture::GetImgSrcPos(const TexturePacker& tp, const d2d::Image* img, int* src)
{
	const TexturePacker::Frame* tp_frame = tp.Query(img->GetFilepath());
	assert(tp_frame);
	for (int i = 0; i < 4; ++i) {
		src[i*2] = floor(tp_frame->dst.tex_coords[i].x + 0.5f);
		src[i*2+1] = floor(tp_frame->dst.tex_coords[i].y + 0.5f);
	}
}

}