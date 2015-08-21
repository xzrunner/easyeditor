#include "BinaryEPE.h"

#define STRINGIFY(A)  #A
#include "trans_old_ejoy2d_data2.lua"

namespace epbin
{

BinaryEPE::BinaryEPE(const std::string& infile)
{
	Load(infile);
}

BinaryEPE::~BinaryEPE()
{

}

void BinaryEPE::Pack(const std::string& outfile, bool compress)
{
	try {
		PackImpl(outfile, compress);
	} catch (Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void BinaryEPE::Load(const std::string& infile)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_settop(L,0);

	int err = luaL_dostring(L, trans_old_ejoy2d_data2);
	if (err) {
		const char *msg = lua_tostring(L,-1);
		std::cout << msg << std::endl;
	}
	lua_getglobal(L, "trans2table");
	lua_pushstring(L, infile.c_str());
	err = lua_pcall(L, 1, 2, 0);
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
		}
		lua_pop(L,1);
	}
}

void BinaryEPE::PackImpl(const std::string& outfile, bool compress)
{

}

}