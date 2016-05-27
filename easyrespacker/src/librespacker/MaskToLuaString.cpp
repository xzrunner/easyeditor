#include "MaskToLuaString.h"
#include "PackMask.h"

#include <easybuilder.h>

#include <limits>

namespace lua = ebuilder::lua;

namespace erespacker
{

void MaskToLuaString::Pack(const PackMask* mask, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + mask->GetFilepath());

	lua::assign_with_end(gen, "type", "\"mask\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(mask->GetSprID()));
	if (!mask->export_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + mask->export_name + "\"");
	}

	lua::connect(gen, 2, 
		lua::assign("base_id", mask->base->GetSprID()),
		lua::assign("mask_id", mask->mask->GetSprID()));

	gen.detab();
	gen.line("},");
}

}