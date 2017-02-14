#ifndef _POLYMESH_SKIN2_TRIANGLES_H_
#define _POLYMESH_SKIN2_TRIANGLES_H_

#include "Skin2Vertex.h"
#include "Skin2Joint.h"

#include <vector>

#include <stdint.h>

namespace pm
{

class MeshTransform;

class Skin2Triangles
{
public:
	uint16_t joint_num;
	uint16_t vert_num;
	uint16_t tri_num;
	uint16_t padding;

	Skin2Vertex* vertices;
	uint16_t*    vertices_joint;

	uint16_t*    triangles;

	Skin2Joint   joints[1];	

public:
	Skin2Triangles();

	static Skin2Triangles* Create(const std::vector<Skin2Joint>& joints,
		const std::vector<int>& vertices,
		const std::vector<sm::vec2>& texcoords,
		const std::vector<int>& triangles);

}; // Skin2Triangles

}

#endif // _POLYMESH_SKIN2_TRIANGLES_H_