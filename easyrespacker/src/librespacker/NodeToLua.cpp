#include "NodeToLua.h"

#include <ee/Math2D.h>

#include <math.h>

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

}