#include "TidCfg.h"

#include <ee/StringHelper.h>

extern "C" {
	#include <lua.h>
 	#include <lualib.h>
 	#include <lauxlib.h>
};

namespace eui
{

TidCfg* TidCfg::m_instance = NULL;

TidCfg* TidCfg::Instance()
{
	if (!m_instance) {
		m_instance = new TidCfg();
	}
	return m_instance;
}

void TidCfg::LoadConfig()
{
	const char* filepath = "texts_CN.lua";

	lua_State* L = luaL_newstate();
// 	luaL_openlibs(L);
// 	lua_settop(L,0);

// 	lua_pushstring(L, filepath);
// 	int err = lua_pcall(L, 1, 0, 0);
// 	if (err) {
// 		const char *msg = lua_tostring(L,-1);
// 		std::cout << msg << std::endl;
// 	}

	// Load file.
	if(luaL_loadfile(L, filepath) || lua_pcall(L, 0, 1, 0))
	{
		printf("Cannot run file\n");
		return;
	}

	if (lua_gettop(L) != 1 || !lua_istable(L, 1)) {
		return;
	}

	lua_pushnil(L);

	while(lua_next(L, -2) != 0)
	{
		if(lua_isstring(L, -1))
		{
			std::string key = lua_tostring(L, -2);
			std::string val = lua_tostring(L, -1);
			ee::StringHelper::ToUtf8(val);
			m_map_tid.insert(std::make_pair(key, val));
		}
		lua_pop(L, 1);
	}
}

std::string TidCfg::Query(const std::string& key) const
{
	std::map<std::string, std::string>::const_iterator itr 
		= m_map_tid.find(key);
	if (itr != m_map_tid.end()) {
		return itr->second;
	} else {
		return "";
	}
}

}