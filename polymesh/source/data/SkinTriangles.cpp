#include "SkinTriangles.h"
#include "pm_define.h"

namespace pm
{

SkinTriangles::SkinTriangles()
	: vert_num(0)
	, tri_num(0)
	, triangles(NULL)
{
}

SkinTriangles* SkinTriangles::Create(const std::vector<SkinVertex>& vertices,
									 const std::vector<int>& triangles)
{
	int head_sz = sizeof(SkinTriangles) + PTR_SIZE_DIFF - sizeof(SkinVertex);
	int sz = head_sz;
	sz += sizeof(SkinVertex) * vertices.size();		// vertices
	sz += sizeof(uint16_t) * triangles.size();		// triangles
	
	uint8_t* ptr = new uint8_t[sz];
	SkinTriangles* ret = new (ptr) SkinTriangles();
	ret->vert_num = vertices.size();
	ret->tri_num = triangles.size();
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		ret->vertices[i] = vertices[i];
	}
	ret->triangles = (uint16_t*)(ptr + (head_sz + sizeof(SkinVertex) * vertices.size()));
	for (int i = 0, n = triangles.size(); i < n; ++i) {
		ret->triangles[i] = triangles[i];
	}

	return ret;
}

}