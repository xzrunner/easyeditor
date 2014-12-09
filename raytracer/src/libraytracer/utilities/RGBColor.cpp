#include "RGBColor.h"

#include <math.h>

namespace rt
{

RGBColor RGBColor::Powc(float p) const
{
	return RGBColor(pow(r, p), pow(g, p), pow(b, p));
}

}