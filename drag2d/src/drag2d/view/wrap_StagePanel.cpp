#include "wrap_StagePanel.h"

#include "dataset/wrap_ISprite.h"
#include "dataset/wrap_SelectionSet.h"
#include "view/MultiSpritesImpl.h"
#include "message/InsertSpriteSJ.h"

d2d::StageModule MODULE_STAGE;

namespace d2d
{
// 	int w_stage_sprite_count(lua_State* L)
// 	{
// 		return 1;
// 	}
// 
// 	int w_get_stage_sprite(lua_State* L)
// 	{
// 		if (MODULE_STAGE.impl) 
// 		{
// 			SpriteSelection* sel = MODULE_STAGE.impl->getSpriteSelection();
// 			if (sel) 
// 			{
// 				std::vector<ISprite*> sprites;
// 				sel->traverse(FetchAllVisitor<ISprite>(sprites));
// 				if (!sprites.empty())
// 				{
// 					luax_newtype(L, "Sprite", SPRITE_DATA_T, (void*)sprites[0]);
// 					return 1;
// 				}
// 			}
// 		}
// 		return 0;
// 	}

	int w_stage_get_selected_sprites(lua_State* L)
	{
		if (!MODULE_STAGE.impl)
			return 0;

		SpriteSelection* sel = MODULE_STAGE.impl->GetSpriteSelection();
		if (!sel)
			return 0;

		luax_newtype(L, "SpriteSelection", SPRITE_SELECTION_DATA_T, (void*)sel);
		return 1;
	}

	int w_stage_move(lua_State* L)
	{
		if (!MODULE_STAGE.impl)
			return 0;

		float x = (float)luaL_checknumber(L, 1);
		float y = (float)luaL_checknumber(L, 2);
		std::vector<ISprite*> sprites;
		MODULE_STAGE.impl->TraverseSprites(FetchAllVisitor<ISprite>(sprites));
		for (int i = 0, n = sprites.size(); i < n; ++i)
		{
			ISprite* s = sprites[i];
			Vector pos = s->GetPosition() + Vector(x, y);
			s->SetTransform(pos, s->GetAngle());
		}
		return 0;		
	}

	int w_stage_insert(lua_State* L)
	{
		if (!MODULE_STAGE.impl)
			return 0;

		ISprite* t = luax_checksprite(L, 1);
		if (t) {
			InsertSpriteSJ::Instance()->Insert(t);
		}
		return 0;
	}

	static const luaL_Reg functions[] = {
		{ "getSelectedSprites", w_stage_get_selected_sprites },
		{ "move", w_stage_move },
		{ "insert", w_stage_insert },
		{ 0, 0 }
	};

	static const lua_CFunction types[] = {
		luaopen_sprite,
		luaopen_SpriteSelection,
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