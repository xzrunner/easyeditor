#include "NodeToBin.h"



#include <math.h>

namespace librespacker
{

int NodeToBin::TransTime(float time)
{
	return floor(time * 1000 + 0.5f);
}

int NodeToBin::TransRadian(float r)
{
	return floor(r * ee::TRANS_RAD_TO_DEG + 0.5f);
}

int NodeToBin::TransFloat(float f)
{
	return floor(f + 0.5f);
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