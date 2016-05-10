#include "ShapeToLuaString.h"
#include "typedef.h"

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

	lua::assign_with_end(gen, "vertices_num", shape->vertices.size());

	std::stringstream ss;
	ss << "vertices = {";
	for (int i = 0, n = shape->vertices.size(); i < n; ++i) {
		const sm::vec2& pos = shape->vertices[i];
		int32_t x = floor(pos.x * SCALE + 0.5f),
			    y =-floor(pos.y * SCALE + 0.5f);
		ss << x << ", " << y << ", ";
	}
	ss << "}";
	gen.line(ss.str());

	gen.detab();
	gen.line("},");
}

}