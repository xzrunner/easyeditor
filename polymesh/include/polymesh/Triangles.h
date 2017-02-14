#ifndef _POLYMESH_TRIANGLES_H_
#define _POLYMESH_TRIANGLES_H_

#include "Vertex.h"

#include <vector>

#include <stdint.h>

namespace pm
{

class MeshTransform;

class Triangles
{
public:
	uint16_t  vert_num;
	uint16_t  tri_num;
	uint16_t* triangles;
	Vertex    vertices[1];

public:
	Triangles();

	static Triangles* Create(const std::vector<sm::vec2>& vertices, 
		const std::vector<sm::vec2>& texcoords, 
		const std::vector<int>& triangles);

}; // Triangles

}

#endif // _POLYMESH_TRIANGLES_H_