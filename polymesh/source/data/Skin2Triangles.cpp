#include "Skin2Triangles.h"
#include "MeshTransform.h"
#include "pm_define.h"

#include <assert.h>

namespace pm
{

Skin2Triangles::Skin2Triangles()
	: joint_num(0)
	, vert_num(0)
	, tri_num(0)
	, vertices(NULL)
	, triangles(NULL)
{
}

Skin2Triangles* Skin2Triangles::Create(const std::vector<Skin2Joint>& joints,
									   const std::vector<int>& vertices,
									   const std::vector<sm::vec2>& texcoords,
									   const std::vector<int>& triangles)
{
	assert(vertices.size() == texcoords.size());

	int head_sz = sizeof(Skin2Triangles) + PTR_SIZE_DIFF * 3 - sizeof(Skin2Joint);
	int sz = head_sz;
	// joints
	sz += sizeof(Skin2Joint) * joints.size();
	// vertices
	sz += sizeof(Skin2Vertex) * vertices.size();
	sz += sizeof(uint16_t) * vertices.size();
	// triangles
	sz += sizeof(uint16_t) * triangles.size();
	
	uint8_t* ptr = new uint8_t[sz];
	Skin2Triangles* ret = new (ptr) Skin2Triangles();
	ret->joint_num = joints.size();
	ret->vert_num  = vertices.size();
	ret->tri_num   = triangles.size();
	// joints
	for (int i = 0, n = joints.size(); i < n; ++i) {
		ret->joints[i] = joints[i];
	}
	// vertices
	ptr += head_sz + sizeof(Skin2Joint) * joints.size();
	ret->vertices = reinterpret_cast<Skin2Vertex*>(ptr);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		Skin2Vertex* v = reinterpret_cast<Skin2Vertex*>(ptr);
		v->xy.Set(0, 0);
		v->uv = texcoords[i];
		ptr += sizeof(Skin2Vertex);
	}
	ret->vertices_joint = reinterpret_cast<uint16_t*>(ptr);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		ret->vertices_joint[i] = vertices[i];
		ptr += sizeof(uint16_t);
	}
	// triangles
	ret->triangles = reinterpret_cast<uint16_t*>(ptr);
	for (int i = 0, n = triangles.size(); i < n; ++i) {
		ret->triangles[i] = triangles[i];
		ptr += sizeof(uint16_t);
	}

	return ret;
}

}