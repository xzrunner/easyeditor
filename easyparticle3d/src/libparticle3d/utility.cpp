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

}