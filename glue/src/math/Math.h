#ifndef _GLUE_MATH_H_
#define _GLUE_MATH_H_

#include "Matrix.h"

namespace glue
{

static const float PI = 3.1415926f;
// const float Pi = 4 * std::atan(1.0f);
// const float TwoPi = 2 * Pi;

class Math
{
public:
	static vec2 TransVector(const vec2& v, const mat4& m);

}; // Math

}

#endif // _GLUE_MATH_H_