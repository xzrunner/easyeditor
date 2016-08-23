#include "Mesh.h"
#include "color_config.h"

#include <ee/Symbol.h>
#include <ee/std_functor.h>
#include <ee/JsonSerializer.h>

#include <shaderlab.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/MeshNode.h>
#include <sprite2/MeshTriangle.h>
#include <SM_Test.h>
#include <SM_Calc.h>

#include <set>
#include <algorithm>

#include <assert.h>

namespace emesh
{

Mesh::Mesh()
	: s2::Mesh()
{
}

Mesh::Mesh(const Mesh& mesh)
	: s2::Mesh(mesh)
{
}

Mesh::Mesh(const ee::Symbol* base)
	: s2::Mesh(base)
{	
}

s2::MeshNode* Mesh::PointQueryNode(const sm::vec2& p)
{
	s2::MeshNode* node = NULL;
	float nearest = FLT_MAX;
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		s2::MeshTriangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j) {
			float dis = sm::dis_pos_to_pos(tri->nodes[j]->xy, p);
			if (dis < m_node_radius && dis < nearest) {
				nearest = dis;
				node = tri->nodes[j];
			}
		}
	}

	return node;
}

void Mesh::RectQueryNodes(const sm::rect& r, std::vector<s2::MeshNode*>& nodes)
{
	std::set<s2::MeshNode*> unique;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		s2::MeshTriangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j) {
			s2::MeshNode* node = tri->nodes[j];
			if (sm::is_point_in_rect(node->xy, r) &&
				unique.find(node) == unique.end()) {
				nodes.push_back(node);
				unique.insert(node);
			}
		}
	}
}

void Mesh::StoreTriangles(Json::Value& value) const
{
	std::vector<sm::vec2> transform;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		s2::MeshTriangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
			transform.push_back(tri->nodes[i]->xy);
	}
	ee::JsonSerializer::Store(transform, value);
}

void Mesh::LoadTriangles(const Json::Value& value)
{
	std::vector<sm::vec2> transform;
	ee::JsonSerializer::Load(value, transform);
	int itr = 0;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		s2::MeshTriangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
			tri->nodes[i]->xy = transform[itr++];
	}
}

}