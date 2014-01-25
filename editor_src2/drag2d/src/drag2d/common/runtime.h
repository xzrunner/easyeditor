#ifndef D2D_RUNTIME_H
#define D2D_RUNTIME_H

#include "tools.h"
#include "types.h"
#include "Module.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

namespace d2d
{
	/**
	* Registries represent special tables which can be accessed with
	* luax_getregistry.
	**/
	enum Registry
	{
		REGISTRY_GC = 1,
		REGISTRY_MODULES
	};

	/**
	* This structure wraps all Lua-exposed objects. It exists in the
	* Lua state as a full userdata (so we can catch __gc "events"),
	* though the Object it refers to is light userdata in the sense
	* that it is not allocated by the Lua VM.
	**/
	struct Proxy
	{
		// Holds type information (see types.h).
		uint32_t flags;

		// The light userdata.
		void * data;

		// True if Lua should delete on GC.
		bool own;
	};

	/**
	* A Module with Lua wrapper functions and other data.
	**/
	struct WrappedModule
	{
		// The module containing the functions.
		Module * module;

		// The name for the table to put the functions in, without the 'd2d'-prefix.
		const char * name;

		// The type flags of this module.
		uint32_t flags;

		// The functions of the module (last element {0,0}).
		const luaL_Reg * functions;

		// A list of functions which expose the types of the modules (last element 0).
		const lua_CFunction * types;

	};

	/**
	 * Registers all functions in the array l (see luaL_Reg) into the table at the
	 * top of the stack.
	 * Similar to Lua 5.2's luaL_setfuncs without the upvalues, and to Lua 5.1's
	 * luaL_register without the library name.
	 **/
	void luax_setfuncs(lua_State *L, const luaL_Reg *l);

	/**
	* Register a module in the d2d table. The d2d table will be created if it does not exist.
	* @param L The Lua state.
	**/
	int luax_register_module(lua_State * L, const WrappedModule & m);

	/**
	* Inserts a module with 'name' into the package.preloaded table.
	* @param f The function to be called when the module is opened.
	* @param name The name of the module, with 'd2d'-prefix, for instance 'd2d.graphics'.
	**/
	int luax_preload(lua_State * L, lua_CFunction f, const char * name);

	/**
	 * Register a new type.
	 * @param tname The name of the type. This must not conflict with other type names,
	 * even from other modules.
	 * @param f The list of member functions for the type.
	 **/
	int luax_register_type(lua_State* L, const char* tname, const luaL_Reg* f = 0);

	/**
	* Creates a new Lua-accessible object of the given type, and put it on the stack.
	* @param L The Lua state.
	* @param name The name of the type. This must match the used earlier with luax_register_type.
	* @param flags The type information.
	* @param data The pointer to the actual object.
	* @own Set this to true (default) if the object should be released upon garbage collection.
	**/
	void luax_newtype(lua_State * L, const char * name, uint32_t flags, void * data, bool own = true);

	/**
	 * 'Insist' that a table 'k' exists in the table at idx. Insistence involves that the
	 * table (k) is created if it does not exist in the table at idx. The table at idx must
	 * pre-exist, however. Also note that if the a non-table value exists at the specified
	 * location, it will be overwritten with a new table. The insisted table, and only the
	 * insisted table, will be placed on top of the stack.
	 *
	 * @param idx The index on the stack containing a table.
	 * @param k The name of the table we are insisting exist.
	 **/
	int luax_insist(lua_State *L, int idx, const char *k);

	/**
	* Insist that a global table 'k' exists. See luax_insist.
	* @param k The name of the table we are insisting exist.
	**/
	int luax_insistglobal(lua_State * L, const char * k);

	/**
	* Insists that a table 'k' exists inside the 'd2d' table. See luax_insist.
	* @param k The name of the table we are insisting exist.
	**/
	int luax_insistlove(lua_State * L, const char * k);

	/**
	* Gets (creates if needed) the specified Registry, and puts it on top
	* of the stack.
	* @param L The Lua state.
	* @param r The Registry to get.
	**/
	int luax_getregistry(lua_State * L, Registry r);

	/**
	 * Like luax_totype, but causes an error if the value at idx is not Proxy,
	 * or is not the specified type.
	 * @param L The Lua state.
	 * @param idx The index on the stack.
	 * @param name The name of the type.
	 * @param type The type bit.
	 **/
	template <typename T>
	T * luax_checktype(lua_State * L, int idx, const char * name, uint32_t type)
	{
		if (lua_isuserdata(L, idx) == 0)
			luaL_error(L, "Incorrect parameter type: expected userdata.");

		Proxy * u = (Proxy *)lua_touserdata(L, idx);

		if ((u->flags & type) != type)
			luaL_error(L, "Incorrect parameter type: expected %s", name);

		return (T *)u->data;
	}
}

#endif // D2D_RUNTIME_H