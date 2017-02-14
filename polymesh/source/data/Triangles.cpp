#include "Triangles.h"
#include "pm_define.h"

#include <assert.h>

namespace pm
{

Triangles::Triangles() 
	: vert_num(0)
	, tri_num(0)
	, triangles(NULL)
{
}

Triangles* Triangles::Create(const std::vector<sm::vec2>& vertices, 
							 const std::vector<sm::vec2>& texcoords, 
							 const std::vector<int>& triangles)
{
	assert(vertices.size() == texcoords.size());
	int head_sz = sizeof(Triangles) + PTR_SIZE_DIFF - sizeof(Vertex);
	int sz = head_sz;
	sz += sizeof(Vertex) * vertices.size();			// vertices
	sz += sizeof(uint16_t) * triangles.size();		// triangles

	uint8_t* ptr = new uint8_t[sz];
	Triangles* ret = new (ptr) Triangles();
	ret->vert_num = vertices.size();
	ret->tri_num = triangles.size();
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		ret->vertices[i].xy = ret->vertices[i].ori_xy = vertices[i];
		ret->vertices[i].uv = texcoords[i];
	}
	ret->triangles = (uint16_t*)(ptr + (head_sz + sizeof(Vertex) * vertices.size()));
	for (int i = 0, n = triangles.size(); i < n; ++i) {
		ret->triangles[i] = triangles[i];
	}

	return ret;
}

}