#ifndef _EASYEDITOR_WRAP_STAGE_PANEL_H_
#define _EASYEDITOR_WRAP_STAGE_PANEL_H_

#include "runtime.h"
#include "Module.h"

namespace ee
{

class MultiSpritesImpl;
struct StageModule : public Module
{
	virtual const char* GetName() const {
		return "d2d.stage";
	}
	MultiSpritesImpl* impl;
};

int w_stage_get_selected_sprites(lua_State* L);
int w_stage_move(lua_State* L);
int w_stage_insert(lua_State* L);

extern "C" __declspec(dllexport) int luaopen_d2d_stage(lua_State* L);

}

#endif // _EASYEDITOR_WRAP_STAGE_PANEL_H_