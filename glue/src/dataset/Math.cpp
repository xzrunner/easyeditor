#include "math/Math.h"

namespace glue
{

vec2 Math::TransVector(const vec2& v, const mat4& m)
{
	vec2 ret;
	const float* e = m.x;
	ret.x = e[0] * v.x + e[4] * v.y + e[12];
	ret.y = e[1] * v.x + e[5] * v.y + e[13];
	return ret;
}

}