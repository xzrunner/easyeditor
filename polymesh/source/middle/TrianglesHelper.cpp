#include "TrianglesHelper.h"
#include "Triangles.h"
#include "MeshTransform.h"

#include <SM_Calc.h>

namespace pm
{

void TrianglesHelper::Dump(const Triangles& src, 
						   std::vector<sm::vec2>& vertices, 
						   std::vector<sm::vec2>& texcoords, 
						   std::vector<int>& triangles)
{
	vertices.resize(src.vert_num);
	texcoords.resize(src.vert_num);
	for (int i = 0; i < src.vert_num; ++i) {
		vertices[i]  = src.vertices[i].xy;
		texcoords[i] = src.vertices[i].uv;
	}

	triangles.resize(src.tri_num);
	for (int i = 0; i < src.tri_num; ++i) {
		triangles[i] = src.triangles[i];
	}
}

void TrianglesHelper::LoadFromTransform(Triangles& dst, const MeshTransform& src)
{
	const std::vector<std::pair<int, sm::vec2> >& trans = src.GetTrans();
	for (int i = 0, n = trans.size(); i < n; ++i) {
		dst.vertices[trans[i].first].xy = dst.vertices[trans[i].first].ori_xy;
	}
	for (int i = 0, n = trans.size(); i < n; ++i) {
		dst.vertices[trans[i].first].xy += trans[i].second;
	}
}

void TrianglesHelper::StoreToTransform(MeshTransform& dst, const Triangles& src)
{
	std::vector<std::pair<int, sm::vec2> > trans;
	for (int i = 0; i < src.vert_num; ++i) {
		const Vertex& v = src.vertices[i];
		if (v.xy != v.ori_xy) {
			trans.push_back(std::make_pair(i, v.xy - v.ori_xy));
		}
	}
	dst.SetTrans(trans);
}

const Vertex* TrianglesHelper::PointQueryVertex(const Triangles& src, const sm::vec2& p, float radius)
{
	const Vertex* ret = NULL;
	float nearest = FLT_MAX;
	for (int i = 0; i < src.vert_num; ++i) {
		float dis = sm::dis_pos_to_pos(src.vertices[i].xy, p);
		if (dis < radius && dis < nearest) {
			nearest = dis;
			ret = &src.vertices[i];
		}
	}
	return ret;
}

void TrianglesHelper::RectQueryVertices(const Triangles& src, const sm::rect& r, std::vector<const Vertex*>& vertices)
{
	for (int i = 0; i < src.vert_num; ++i) {
		if (sm::is_point_in_rect(src.vertices[i].xy, r)) {
			vertices.push_back(&src.vertices[i]);
		}
	}
}

const sm::vec2* TrianglesHelper::GetVertexPos(const Triangles& src, int idx)
{
	if (idx < 0 || idx >= src.vert_num) {
		return NULL;
	} else {
		return &src.vertices[idx].xy;
	}
}

void TrianglesHelper::SetVertexPos(Triangles& src, int idx, const sm::vec2& pos)
{
	if (idx >= 0 && idx < src.vert_num) {
		src.vertices[idx].xy = pos;
	}
}

}