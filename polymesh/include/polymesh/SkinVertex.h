#ifndef _POLYMESH_SKIN_VERTEX_H_
#define _POLYMESH_SKIN_VERTEX_H_

#include <SM_Vector.h>

#include <stdint.h>

namespace pm
{

class SkinVertex
{
public:
	sm::vec2 xy, uv;
	sm::vec2 ori_xy;
	int16_t  joint_id[4];
	float    joint_weight[3];

public:
	SkinVertex();

}; // SkinVertex

}

#endif // _POLYMESH_SKIN_VERTEX_H_