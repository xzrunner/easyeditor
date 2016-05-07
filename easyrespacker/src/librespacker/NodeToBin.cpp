#include "NodeToBin.h"

#include <ee/Math2D.h>

#include <math.h>

namespace erespacker
{

int NodeToBin::TransTime(float time)
{
	return static_cast<int>(floor(time * 1000 + 0.5f));
}

int NodeToBin::TransRadian(float r)
{
	return static_cast<int>(floor(r * SM_RAD_TO_DEG + 0.5f));
}

int NodeToBin::TransFloat(float f)
{
	return static_cast<int>(floor(f + 0.5f));
}

int NodeToBin::TransFloatX100(float f)
{
	return TransFloat(100 * f);
}

int NodeToBin::TransBool(bool b)
{
	return b ? 1 : 0;
}

}