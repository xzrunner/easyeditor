#include "ShapeToLuaString.h"
#include "typedef.h"
#include "PackShape.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace erespacker
{

void ShapeToLuaString::Pack(const PackShape* shape, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + shape->GetFilepath());

	lua::assign_with_end(gen, "type", "\"shape\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(shape->GetSprID()));

	lua::assign_with_end(gen, "shape_type", shape->type);
	lua::assign_with_end(gen, "color", shape->color.ToRGBA());

	PackVertices(shape->vertices, gen);

	gen.detab();
	gen.line("},");
}

}