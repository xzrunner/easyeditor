#ifndef _EASYPARTICLE3D_UTILITY_H_
#define _EASYPARTICLE3D_UTILITY_H_

#include <SM_Matrix.h>
#include <sprite2/Color.h>
#include <sprite2/pre_defined.h>
#include S2_MAT_HEADER

#include <ps_utility.h>

namespace eparticle3d
{

inline sm::vec2 TransCoords3To2(const float position[3])
{
	float gx = position[0] * 0.01f,
		gy = position[1] * 0.01f;
	float x = (gx - gy) * 36,
		y = (gx + gy) * 26 + position[2] * 0.5f;
	return sm::vec2(x, y);
}

inline sm::vec2 TransCoords3To2(const float position[3], const S2_MAT& direction)
{
// 	sm::vec3 trans_pos3 = direction * sm::vec3(position[0], position[1], position[2]);
// 	float gx = trans_pos3.x * 0.01f,
// 		  gy = trans_pos3.y * 0.01f;
// 	float x = (gx - gy) * 36,
// 		  y = (gx + gy) * 26 + trans_pos3.z * 0.5f;
// 	return sm::vec2(x, y);

	//////////////////////////////////////////////////////////////////////////

	return sm::vec2(0, 0);
}

inline ps_color ColorFromS2(const s2::Color& col)
{
	ps_color ret;
	ret.r = col.r;
	ret.g = col.g;
	ret.b = col.b;
	ret.a = col.a;
	return ret;
}

}

#endif // _EASYPARTICLE3D_UTILITY_H_