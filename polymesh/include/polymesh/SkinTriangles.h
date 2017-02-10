#ifndef _POLYMESH_SKIN_TRIANGLES_H_
#define _POLYMESH_SKIN_TRIANGLES_H_

#include "SkinVertex.h"

#include <vector>

namespace pm
{

class SkinTriangles
{
public:
	uint16_t   vert_num;
	uint16_t   tri_num;
	uint16_t*  triangles;
	SkinVertex vertices[1];
	
public:
	SkinTriangles();

	static SkinTriangles* Create(const std::vector<SkinVertex>& vertices,
								 const std::vector<int>& triangles);

}; // SkinTriangles

}

#endif // _POLYMESH_SKIN_TRIANGLES_H_