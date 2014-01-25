#include "wrap_ISprite.h"

namespace d2d
{
	ISprite* luax_checksprite(lua_State* L, int idx)
	{
		return luax_checktype<ISprite>(L, idx, "Sprite", SPRITE_DATA_T);
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

	static const luaL_Reg functions[] = {
		{ "getPosition", w_Sprite_getPosition },
		{ "setPosition", w_Sprite_setPosition },
		{ 0, 0 }
	};

	extern "C" int luaopen_sprite(lua_State* L)
	{
		return luax_register_type(L, "Sprite", functions);
	}
}