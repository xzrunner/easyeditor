#include "wrap_ISprite.h"
#include "ISymbol.h"

namespace d2d
{
	ISprite* luax_checksprite(lua_State* L, int idx)
	{
		return luax_checktype<ISprite>(L, idx, "Sprite", SPRITE_DATA_T);
	}

	int w_Sprite_clone(lua_State* L)
	{
		ISprite* t = luax_checksprite(L, 1);
		ISprite* clone = t->clone();
		luax_newtype(L, "Sprite", SPRITE_DATA_T, (void*)clone);
		return 1;
	}

	int w_Sprite_getPosition(lua_State* L)
	{
		ISprite* t = luax_checksprite(L, 1);
		const Vector& pos = t->getPosition();
		lua_pushnumber(L, pos.x);
		lua_pushnumber(L, pos.y);
		return 2;
	}

	int w_Sprite_setPosition(lua_State* L)
	{
		ISprite* t = luax_checksprite(L, 1);
		float x = (float)luaL_checknumber(L, 2);
		float y = (float)luaL_checknumber(L, 3);
		t->setTransform(Vector(x, y), t->getAngle());
		return 0;
	}

	int w_Sprite_move(lua_State* L)
	{
		ISprite* t = luax_checksprite(L, 1);
		float x = (float)luaL_checknumber(L, 2);
		float y = (float)luaL_checknumber(L, 3);
		t->setTransform(t->getPosition() + Vector(x, y), t->getAngle());
		return 0;
	}

	int w_Sprite_width(lua_State* L)
	{
		ISprite* t = luax_checksprite(L, 1);
		lua_pushnumber(L, t->getSymbol().getSize().xLength());
		return 1;
	}

	int w_Sprite_height(lua_State* L)
	{
		ISprite* t = luax_checksprite(L, 1);
		lua_pushnumber(L, t->getSymbol().getSize().yLength());
		return 1;
	}

	int w_Sprite_setName(lua_State* L)
	{
		ISprite* t = luax_checksprite(L, 1);
		const char* str = luaL_checkstring(L, 2);
		t->name = str;
		return 0;
	}

	static const luaL_Reg functions[] = {
		{ "clone", w_Sprite_clone },
		{ "getPosition", w_Sprite_getPosition },
		{ "setPosition", w_Sprite_setPosition },
		{ "move", w_Sprite_move },
		{ "width", w_Sprite_width },
		{ "height", w_Sprite_height },
		{ "setName", w_Sprite_setName },
		{ 0, 0 }
	};

	extern "C" int luaopen_sprite(lua_State* L)
	{
		return luax_register_type(L, "Sprite", functions);
	}
}