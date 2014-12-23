#include "BinaryEPD.h"
#include "LuaDataHelper.h"
#include "epd_dataset.h"
#include "tools.h"
#include "Exception.h"
#include "Lzma.h"

#define STRINGIFY(A)  #A
#include "trans_old_ejoy2d_data.lua"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

#include <iostream>
#include <string>
#include <algorithm>
#include <assert.h>

#include <eploader.h>

namespace epbin
{

static const uint8_t DATA = 2;

BinaryEPD::BinaryEPD(const std::string& src, 
			   const std::string& dst)
	: m_src_filepath(src)
	, m_max_id(0)
	, m_export(0)
	, m_fout(dst.c_str(), std::ios::binary)
{
	Load();
}

BinaryEPD::~BinaryEPD()
{
	m_fout.close();	

	for_each(m_pictures.begin(), m_pictures.end(), DeletePointerFunctor<Picture>());
	for_each(m_animations.begin(), m_animations.end(), DeletePointerFunctor<Animation>());
}

void BinaryEPD::Pack(bool compress)
{
	// compute ejoypic size
	size_t ep_sz = 0;
	for (int i = 0, n = m_pictures.size(); i < n; ++i) {
		ep_sz += m_pictures[i]->Size();
	}
	for (int i = 0, n = m_animations.size(); i < n; ++i) {
		ep_sz += m_animations[i]->Size();
	}
	ep_sz += sizeof(m_max_id);
	ep_sz += sizeof(m_export);

	// ejoypic buffer
	uint8_t* ep_buf = new uint8_t[ep_sz];
	uint8_t* ptr_ep = ep_buf;
	// fill ep_buf
	for (int i = 0, n = m_pictures.size(); i < n; ++i) {
		m_pictures[i]->Store(&ptr_ep);
	}
	for (int i = 0, n = m_animations.size(); i < n; ++i) {
		m_animations[i]->Store(&ptr_ep);
	}
	memcpy(ptr_ep, &m_max_id, sizeof(m_max_id));
	ptr_ep += sizeof(m_max_id);
	memcpy(ptr_ep, &m_export, sizeof(m_export));
	ptr_ep += sizeof(m_export);

	assert(ptr_ep - ep_buf == ep_sz);

	int cap = ejoypic_capacity(ep_buf, ep_sz);

	// final
	size_t sz = ep_sz + sizeof(uint8_t) + sizeof(uint32_t);
	uint8_t* buf = new uint8_t[sz];
	uint8_t* ptr = buf;
	memcpy(ptr, &DATA, sizeof(uint8_t));
	ptr += sizeof(uint8_t);
	memcpy(ptr, &cap, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	memcpy(ptr, ep_buf, ep_sz);
	delete[] ep_buf;

	// compress
	if (compress)
	{
		uint8_t* dst = NULL;
		size_t dst_sz;
		Lzma::Compress(&dst, &dst_sz, buf, sz);

		m_fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
		m_fout.write(reinterpret_cast<const char*>(dst), dst_sz);
	}
	else
	{
		int _sz = -(int)sz;
		m_fout.write(reinterpret_cast<const char*>(&_sz), sizeof(int32_t));
		m_fout.write(reinterpret_cast<const char*>(buf), sz);
	}

	delete[] buf;
}

void BinaryEPD::Load()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_settop(L,0);

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

	if (lua_gettop(L) != 1 || !lua_istable(L, 1)) {
		return;
	}

	int len = lua_rawlen(L, 1);
	for(int i = 1; i <= len; i++)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, 1);

		if (lua_istable(L, -1))
		{
			std::string type = LuaDataHelper::GetStringField(L, "type");
			int id = LuaDataHelper::GetIntField(L, "id");
			CheckID(id);
			if (type == "picture") {
				m_pictures.push_back(new Picture(L, id));
			} else if (type == "animation") {
				m_animations.push_back(new Animation(L, id));
			} else {
				assert(0);
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

}