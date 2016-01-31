#include "runtime.h"
#include "Module.h"

namespace ee
{

void luax_setfuncs(lua_State *L, const luaL_Reg *l)
{
	if (l == 0)
		return;

	for (; l->name != 0; l++)
	{
		lua_pushcfunction(L, l->func);
		lua_setfield(L, -2, l->name);
	}
}

int luax_register_module(lua_State * L, const WrappedModule & m)
{
	// Put a reference to the C++ module in Lua.
	luax_getregistry(L, REGISTRY_MODULES);

	Proxy * p = (Proxy *)lua_newuserdata(L, sizeof(Proxy));
	p->own = true;
	p->data = m.module;
	p->flags = m.flags;

	luaL_newmetatable(L, m.module->GetName());
	//lua_pushvalue(L, -1);
	//lua_setfield(L, -2, "__index");
	//lua_pushcfunction(L, w__gc);
	//lua_setfield(L, -2, "__gc");

	lua_setmetatable(L, -2);
	lua_setfield(L, -2, m.name); // _modules[name] = proxy
	lua_pop(L, 1);

	// Gets the d2d table.
	luax_insistglobal(L, "d2d");

	// Create new table for module.
	lua_newtable(L);

	// Register all the functions.
	if (m.functions != 0)
		luax_setfuncs(L, m.functions);

	// Register types.
	if (m.types != 0)
		for (const lua_CFunction * t = m.types; *t != 0; t++)
			(*t)(L);

	lua_pushvalue(L, -1);
	lua_setfield(L, -3, m.name); // d2d.graphics = table
	lua_remove(L, -2); // d2d

	return 1;
}

int luax_preload(lua_State * L, lua_CFunction f, const char * name)
{
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");
	lua_pushcfunction(L, f);
	lua_setfield(L, -2, name);
	lua_pop(L, 2);
	return 0;
}

int luax_register_type(lua_State* L, const char* tname, const luaL_Reg* f)
{
	luaL_newmetatable(L, tname);

	// m.__index = m
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

// 	// setup gc
// 	lua_pushcfunction(L, w__gc);
// 	lua_setfield(L, -2, "__gc");

// 	// Add equality
// 	lua_pushcfunction(L, w__eq);
// 	lua_setfield(L, -2, "__eq");

// 	// Add tostring function.
// 	lua_pushstring(L, tname);
// 	lua_pushcclosure(L, w__tostring, 1);
// 	lua_setfield(L, -2, "__tostring");
// 
// 	// Add tostring to as type() as well.
// 	lua_pushstring(L, tname);
// 	lua_pushcclosure(L, w__tostring, 1);
// 	lua_setfield(L, -2, "type");
// 
// 	// Add typeOf
// 	lua_pushcfunction(L, w__typeOf);
// 	lua_setfield(L, -2, "typeOf");

	if (f != 0)
		luax_setfuncs(L, f);

	lua_pop(L, 1); // Pops metatable.
	return 0;
}

void luax_newtype(lua_State * L, const char * name, uint32_t flags, void * data, bool own)
{
	Proxy * u = (Proxy *)lua_newuserdata(L, sizeof(Proxy));

	u->data = data;
	u->flags = flags;
	u->own = own;

	luaL_newmetatable(L, name);
	lua_setmetatable(L, -2);
}

int luax_insist(lua_State *L, int idx, const char *k)
{
	// Convert to absolute index if necessary.
	if (idx < 0 && idx > LUA_REGISTRYINDEX)
		idx = lua_gettop(L) + ++idx;

	lua_getfield(L, idx, k);

	// Create if necessary.
	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1); // Pop the non-table.
		lua_newtable(L);
		lua_pushvalue(L, -1); // Duplicate the table to leave on top.
		lua_setfield(L, idx, k); // lua_stack[idx][k] = lua_stack[-1] (table)
	}

	return 1;
}

int luax_insistglobal(lua_State * L, const char * k)
{
	lua_getglobal(L, k);

	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1); // Pop the non-table.
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setglobal(L, k);
	}

	return 1;
}

int luax_insistlove(lua_State * L, const char * k)
{
	luax_insistglobal(L, "d2d");
	luax_insist(L, -1, k);

	// The d2d table should be replaced with the top stack
	// item. Only the reqested table should remain on the stack.
	lua_replace(L, -2);

	return 1;
}

int luax_getregistry(lua_State * L, Registry r)
{
	switch(r)
	{
	case REGISTRY_GC:
		return luax_insistlove(L, "_gc");
	case REGISTRY_MODULES:
		return luax_insistlove(L, "_modules");
	default:
		return luaL_error(L, "Attempted to use invalid registry.");
	}
}

int luax_insistregistry(lua_State *L, Registry r)
{
	switch (r)
	{
	case REGISTRY_GC:
		return luax_insistlove(L, "_gc");
	case REGISTRY_MODULES:
		return luax_insistlove(L, "_modules");
	default:
		return luaL_error(L, "Attempted to use invalid registry.");
	}
}

}