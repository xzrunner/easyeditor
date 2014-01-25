#include "wrap_StagePanel.h"

#include "dataset/wrap_ISprite.h"

#include "dataset/SelectionSet.h"
#include "view/MultiSpritesImpl.h"

d2d::StageModule MODULE_STAGE;

namespace d2d
{
	int w_get_stage_sprite(lua_State* L)
	{
		if (MODULE_STAGE.impl) 
		{
			SpriteSelection* sel = MODULE_STAGE.impl->getSpriteSelection();
			if (sel) 
			{
				std::vector<ISprite*> sprites;
				sel->traverse(FetchAllVisitor<ISprite>(sprites));
				if (!sprites.empty())
				{
					luax_newtype(L, "Sprite", SPRITE_DATA_T, (void*)sprites[0]);
					return 1;
				}
			}
		}
		return 0;
	}

	static const luaL_Reg functions[] = {
		{ "getSprite", w_get_stage_sprite },
		{ 0, 0 }
	};

	static const lua_CFunction types[] = {
		luaopen_sprite,
		0
	};

	extern "C" __declspec(dllexport) int luaopen_d2d_stage(lua_State* L)
	{
		WrappedModule w;
		w.module = &MODULE_STAGE;
		w.name = "stage";
		w.flags = MODULE_T;
		w.functions = functions;
		w.types = types;

		int n = luax_register_module(L, w);

		return n;
	}
}