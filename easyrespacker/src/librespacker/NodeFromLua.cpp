#include "NodeFromLua.h"

#include <math.h>



namespace librespacker
{

float NodeFromLua::TransTime(int time)
{
	return time * 0.001f;
}

float NodeFromLua::TransDegree(int deg)
{
	return deg * ee::TRANS_DEG_TO_RAD;
}

float NodeFromLua::TransFloatX100(int f)
{
	return f * 0.01f;
}

}