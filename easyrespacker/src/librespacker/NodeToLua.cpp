#include "NodeToLua.h"

#include <math.h>

#include <drag2d.h>

namespace librespacker
{

int NodeToLua::TransTime(float time)
{
	return floor(time * 1000 + 0.5f);
}

int NodeToLua::TransRadian(float r)
{
	return floor(r * d2d::TRANS_RAD_TO_DEG + 0.5f);
}

int NodeToLua::TransFloat(float f)
{
	return floor(f + 0.5f);
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