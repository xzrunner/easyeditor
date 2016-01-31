#include "P3dSprToLuaString.h"
#include "PackP3dSpr.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace librespacker
{

void P3dSprToLuaString::Pack(const PackP3dSpr* spr, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + spr->GetFilepath());

	lua::assign_with_end(gen, "type", "\"p3d_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(spr->GetSprID()));
	if (!spr->export_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + spr->export_name + "\"");
	}

	lua::connect(gen, 1, 
		lua::assign("p3d_id", spr->p3d->GetSprID()));

	lua::connect(gen, 4, 
		lua::assign("is_loop", TransBool(spr->loop)), 
		lua::assign("is_local", TransBool(spr->local)), 
		lua::assign("is_alone", TransBool(spr->alone)), 
		lua::assign("is_reuse", TransBool(spr->reuse)));

	gen.detab();
	gen.line("},");
}

}