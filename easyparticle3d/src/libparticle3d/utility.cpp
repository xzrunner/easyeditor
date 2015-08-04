#include "Utility.h"

namespace eparticle3d
{

d2d::Vector TransCoords3To2(const float position[3])
{
	float gx = position[0] * 0.01f,
		  gy = position[1] * 0.01f;
	float x = (gx - gy) * 36,
		  y = (gx + gy) * 26 + position[2] * 0.5f;
	return d2d::Vector(x, y);
}

d2d::Vector TransCoords3To2(const float position[3], const mat4& direction)
{
	vec3 trans_pos3 = direction * vec3(position[0], position[1], position[2]);
	float gx = trans_pos3.x * 0.01f,
		gy = trans_pos3.y * 0.01f;
	float x = (gx - gy) * 36,
		y = (gx + gy) * 26 + trans_pos3.z * 0.5f;
	return d2d::Vector(x, y);
}

}