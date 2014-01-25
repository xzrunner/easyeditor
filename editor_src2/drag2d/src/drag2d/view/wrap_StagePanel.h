#ifndef D2D_WRAP_STAGE_PANEL_H
#define D2D_WRAP_STAGE_PANEL_H

#include "common/runtime.h"

namespace d2d
{
	class MultiSpritesImpl;
	struct StageModule : public Module
	{
		virtual const char* getName() const {
			return "d2d.stage";
		}
		MultiSpritesImpl* impl;
	};

	int w_stage_get_selected_sprites(lua_State* L);
	int w_stage_move(lua_State* L);
	int w_stage_insert(lua_State* L);

	extern "C" __declspec(dllexport) int luaopen_d2d_stage(lua_State* L);
}

#endif // D2D_WRAP_STAGE_PANEL_H