#ifndef _EASYPARTICLE3D_UTILITY_H_
#define _EASYPARTICLE3D_UTILITY_H_

#include <ee/Vector.h>
#include <ee/Matrix3D.h>

namespace eparticle3d
{

inline ee::Vector TransCoords3To2(const float position[3])
{
	float gx = position[0] * 0.01f,
		gy = position[1] * 0.01f;
	float x = (gx - gy) * 36,
		y = (gx + gy) * 26 + position[2] * 0.5f;
	return ee::Vector(x, y);
}

inline ee::Vector TransCoords3To2(const float position[3], const ee::mat4& direction)
{
	ee::vec3 trans_pos3 = direction * ee::vec3(position[0], position[1], position[2]);
	float gx = trans_pos3.x * 0.01f,
		gy = trans_pos3.y * 0.01f;
	float x = (gx - gy) * 36,
		y = (gx + gy) * 26 + trans_pos3.z * 0.5f;
	return ee::Vector(x, y);
}

}

#endif // _EASYPARTICLE3D_UTILITY_H_