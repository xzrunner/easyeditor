#include "NodeFromLua.h"

#include <math.h>

#include <drag2d.h>

namespace librespacker
{

float NodeFromLua::TransTime(int time)
{
	return time * 0.001f;
}

float NodeFromLua::TransDegree(int deg)
{
	return deg * d2d::TRANS_DEG_TO_RAD;
}

float NodeFromLua::TransFloatX100(int f)
{
	return f * 0.01f;
}

}