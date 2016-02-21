#include "P3dSprFromLua.h"
#include "PackP3dSpr.h"
#include "UnpackNodeFactory.h"
#include "LuaDataHelper.h"

namespace erespacker
{

void P3dSprFromLua::Unpack(lua_State* L, PackP3dSpr* spr)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();
	int id = LuaDataHelper::GetIntField(L, "id");
	spr->p3d = factory->Query(id);
	if (!spr->p3d) {
		factory->AddUnassigned(id, &spr->p3d);
	}

	spr->loop = LuaDataHelper::GetBoolField(L, "is_loop");
	spr->local = LuaDataHelper::GetBoolField(L, "is_local");
	spr->alone = LuaDataHelper::GetBoolField(L, "is_alone");
	spr->reuse = LuaDataHelper::GetBoolField(L, "is_reuse");
}

}