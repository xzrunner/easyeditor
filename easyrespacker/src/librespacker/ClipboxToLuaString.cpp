#include "ClipboxToLuaString.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace librespacker
{

void ClipboxToLuaString::Pack(const PackClipbox* cb, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + cb->GetFilepath());

	lua::assign_with_end(gen, "type", "\"pannel\"");
	lua::assign_with_end(gen, "id", d2d::StringHelper::ToString(cb->GetSprID()));

	std::ostringstream ss;
	ss << "x = " << cb->x << ", ";
	ss << "y = " << cb->y << ", ";
	ss << "w = " << cb->w << ", ";
	ss << "h = " << cb->h << ", ";

	gen.line(ss.str());

	gen.detab();
	gen.line("},");
}

}