#ifndef _RT_CONSTANTS_H_
#define _RT_CONSTANTS_H_

#include "RGBColor.h"

namespace rt
{

const double 	PI 			= 3.1415926535897932384;
const double 	TWO_PI 		= 6.2831853071795864769;
const double 	PI_ON_180 	= 0.0174532925199432957;
const double 	INV_PI 		= 0.3183098861837906715;
const double 	INV_TWO_PI 	= 0.1591549430918953358;

//const double	EPSILON		= 0.0001;
const double	EPSILON		= 0.001;

const RGBColor BLACK(0, 0, 0);
const RGBColor WHITE(1, 1, 1);
const RGBColor RED(1, 0, 0);

}

#endif // _RT_CONSTANTS_H_