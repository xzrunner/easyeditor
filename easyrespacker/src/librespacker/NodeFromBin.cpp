#include "NodeFromBin.h"

#include <ee/Math2D.h>

namespace erespacker
{

float NodeFromBin::TransTime(int time)
{
	return time * 0.001f;
}

float NodeFromBin::TransDegree(int deg)
{
	return deg * SM_DEG_TO_RAD;
}

float NodeFromBin::TransFloatX100(int f)
{
	return f * 0.01f;
}

bool NodeFromBin::TransBool(int b)
{
	return b == 0 ? false : true;
}

}