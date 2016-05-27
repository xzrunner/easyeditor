#include "MaskFromLua.h"
#include "PackMask.h"
#include "UnpackNodeFactory.h"
#include "LuaDataHelper.h"

#include <limits>

namespace erespacker
{

void MaskFromLua::Unpack(lua_State* L, PackMask* mask)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	int base_id = LuaDataHelper::GetIntField(L, "base_id");
	mask->base = factory->Query(base_id);
	if (!mask->base) {
		factory->AddUnassigned(base_id, &mask->base);
	}

	int mask_id = LuaDataHelper::GetIntField(L, "mask_id");
	mask->mask = factory->Query(mask_id);
	if (!mask->mask) {
		factory->AddUnassigned(mask_id, &mask->mask);
	}
}

}