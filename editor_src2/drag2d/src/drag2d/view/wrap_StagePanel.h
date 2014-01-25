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

	int w_get_stage_sprite(lua_State* L);
	extern "C" __declspec(dllexport) int luaopen_d2d_stage(lua_State* L);
}

#endif // D2D_WRAP_STAGE_PANEL_H