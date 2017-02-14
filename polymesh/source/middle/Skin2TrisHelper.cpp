#include "Skin2TrisHelper.h"
#include "Skin2Triangles.h"
#include "MeshTransform.h"

#include <SM_Calc.h>

namespace pm
{

void Skin2TrisHelper::Dump(const Skin2Triangles& src, 
						   std::vector<sm::vec2>& vertices, 
						   std::vector<sm::vec2>& texcoords, 
						   std::vector<int>& triangles)
{
	vertices.resize(src.vert_num);
	texcoords.resize(src.vert_num);
	for (int i = 0; i < src.vert_num; ++i) {
		const Skin2Vertex& v = src.vertices[i];
		vertices[i]  = v.xy;
		texcoords[i] = v.uv;
	}

	triangles.resize(src.tri_num);
	for (int i = 0; i < src.tri_num; ++i) {
		triangles[i] = src.triangles[i];
	}
}

void Skin2TrisHelper::LoadFromTransform(Skin2Triangles& dst, const MeshTransform& src)
{
	const std::vector<std::pair<int, sm::vec2> >& trans = src.GetTrans();
	for (int i = 0, n = trans.size(); i < n; ++i) 
	{
		int vert_idx = trans[i].first;
		const sm::vec2& offset = trans[i].second;
		const Skin2Vertex& v = dst.vertices[vert_idx];
		for (int j = 0, m = dst.vertices_joint[vert_idx]; j < m; ++j) {
			Skin2Joint& p = dst.joints[dst.vertices_joint[j]];
			p.offset = offset * p.weight;
		}
	}
}

void Skin2TrisHelper::StoreToTransform(MeshTransform& dst, const Skin2Triangles& src)
{
	std::vector<std::pair<int, sm::vec2> > trans;
	for (int i = 0; i < src.vert_num; ++i)
	{
		const Skin2Vertex& v = src.vertices[i];
		sm::vec2 offset(0, 0);
		for (int j = 0, m = src.vertices_joint[i]; j < m; ++j) {
			const Skin2Joint& p = src.joints[src.vertices_joint[j]];
			offset += p.offset * p.weight;
		}
		if (offset != sm::vec2(0, 0)) {
			trans.push_back(std::make_pair(i, offset));
		}
	}
	dst.SetTrans(trans);
}

const Skin2Vertex* Skin2TrisHelper::PointQueryVertex(const Skin2Triangles& src, const sm::vec2& p, float radius)
{
	const Skin2Vertex* ret = NULL;
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

void Skin2TrisHelper::RectQueryVertices(const Skin2Triangles& src, const sm::rect& r, std::vector<const Skin2Vertex*>& vertices)
{
	for (int i = 0; i < src.vert_num; ++i) {
		if (sm::is_point_in_rect(src.vertices[i].xy, r)) {
			vertices.push_back(&src.vertices[i]);
		}
	}
}

const sm::vec2* Skin2TrisHelper::GetVertexPos(const Skin2Triangles& src, int idx)
{
	if (idx < 0 || idx >= src.vert_num) {
		return NULL;
	} else {
		return &src.vertices[idx].xy;
	}
}

void Skin2TrisHelper::SetVertexPos(Skin2Triangles& src, int idx, const sm::vec2& pos)
{
	if (idx >= 0 && idx < src.vert_num) {
		src.vertices[idx].xy = pos;
	}
}

}