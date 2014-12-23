#include "BinaryEPD.h"
#include "LuaDataHelper.h"
#include "epd_dataset.h"

#define STRINGIFY(A)  #A
#include "trans_old_ejoy2d_data.lua"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

#include <iostream>
#include <string>
#include <assert.h>

namespace epbin
{

static const int ANIMATION	= 0;
static const int PICTURE	= 1;
static const int CLIPUI		= 2;

BinaryEPD::BinaryEPD(const std::string& src, 
			   const std::string& dst)
	: m_src_filepath(src)
	, m_max_id(0)
	, m_export(0)
	, m_fout(dst.c_str(), std::ios::binary)
{
}

BinaryEPD::~BinaryEPD()
{
	m_fout.close();	
}

void BinaryEPD::Pack()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_settop(L,0);

// 	// test if is old format
// 	luaL_dofile(L, m_filename.c_str());
// 	if (lua_isstring(L, -1))
	{
		lua_pop(L, 1);
		int err = luaL_dostring(L, trans_old_ejoy2d_data);
		if (err) {
			const char *msg = lua_tostring(L,-1);
			std::cout << msg << std::endl;
		}
		lua_getglobal(L, "trans2table");
		lua_pushstring(L, m_src_filepath.c_str());
		err = lua_pcall(L, 1, 1, 0);
		if (err) {
			const char *msg = lua_tostring(L,-1);
			std::cout << msg << std::endl;
		}
	}

	if (lua_gettop(L) != 1 || !lua_istable(L, 1)) {
		return;
	}

	int len = lua_rawlen(L, 1);
	for(int i=1;i <= len; i++)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, 1);

		if (lua_istable(L, -1))
		{
			std::string type = LuaDataHelper::GetStringField(L, "type");
			int id = LuaDataHelper::GetIntField(L, "id");
			CheckID(id);
 			if (type == "picture") {
 				BinPic(L, id);
 			} else if (type == "animation") {
 				BinAni(L, id);
 			}
		}
		lua_pop(L,1);
	}
}

void BinaryEPD::CheckID(int id)
{
	if (id > m_max_id) {
		m_max_id = id;
	}
}

void BinaryEPD::BinPic(lua_State* L, int id)
{
	Picture* pic = new Picture;
	pic->m_id = id;
	int len = lua_rawlen(L, -1);
	pic->m_parts.reserve(len);
	for (int i = 1; i <= len; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		Picture::Part part;
		// tex
		lua_getfield(L, -1, "tex");
		const char* type = lua_typename(L, lua_type(L, -1));
		part.tex = (uint8_t)lua_tointeger(L, -1);
		lua_pop(L, 1);
		// src
		lua_getfield(L, -1, "src");
		int len = lua_rawlen(L, -1);
		assert(len == 8);
		uint16_t* src = &part.src[0];
		for (int i = 1; i <= len; ++i)
		{
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			src[i-1] = (uint16_t)lua_tonumber(L, -1);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		// screen
		lua_getfield(L, -1, "screen");
		len = lua_rawlen(L, -1);
		assert(len == 8);
		int32_t* dst = &part.screen[0];
		for (int i = 1; i <= len; ++i)
		{
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			dst[i-1] = (int32_t)lua_tonumber(L, -1);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);

		pic->m_parts.push_back(part);

		lua_pop(L, 1);
	}
	m_pictures.push_back(pic);
}

void BinaryEPD::BinAni(lua_State* L, int id)
{
	
}

}