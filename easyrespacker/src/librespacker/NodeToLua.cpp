#include "NodeToLua.h"
#include "typedef.h"

#include <ee/Math2D.h>

#include <easybuilder.h>

#include <math.h>
#include <stdint.h>

namespace lua = ebuilder::lua;

namespace erespacker
{

int NodeToLua::TransTime(float time)
{
	return static_cast<int>(floor(time * 1000 + 0.5f));
}

int NodeToLua::TransRadian(float r)
{
	return static_cast<int>(floor(r * SM_RAD_TO_DEG + 0.5f));
}

int NodeToLua::TransFloat(float f)
{
	return static_cast<int>(floor(f + 0.5f));
}

int NodeToLua::TransFloatX100(float f)
{
	return TransFloat(100 * f);
}

std::string NodeToLua::TransBool(bool b)
{
	return b ? "true" : "false";
}

void NodeToLua::PackVertices(const std::vector<sm::vec2>& vertices, 
							 ebuilder::CodeGenerator& gen)
{
	lua::assign_with_end(gen, "vertices_num", vertices.size());

	std::stringstream ss;
	ss << "vertices = {";
	for (int i = 0, n = vertices.size(); i < n; ++i) 
	{
		const sm::vec2& pos = vertices[i];
		int32_t x = floor(pos.x * SCALE + 0.5f),
			    y =-floor(pos.y * SCALE + 0.5f);
		ss << x << ", " << y << ", ";
	}
	ss << "}";
	gen.line(ss.str());
}

}