#include "MeshTrans.h"
#include "Mesh.h"
#include "Triangle.h"

#include <ee/JsonSerializer.h>

#include <set>

namespace emesh
{

void MeshTrans::Load(const Json::Value& value)
{
	m_map.clear();
	std::vector<sm::vec2> from, to;
	ee::JsonSerializer::Load(value["trans"]["from"], from);
	ee::JsonSerializer::Load(value["trans"]["to"], to);
	for (int i = 0, n = from.size(); i < n; ++i) {
		m_map.insert(std::make_pair(from[i], to[i]));
	}
}

void MeshTrans::Store(Json::Value& value) const
{
	Json::Value& trans_val = value["trans"];

	int count = 0;
	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr 
		= m_map.begin();
	for ( ; itr != m_map.end(); ++itr) 
	{
		trans_val["from"][count * 2] = itr->first.x;
		trans_val["from"][count * 2 + 1] = itr->first.y;
		trans_val["to"][count * 2] = itr->second.x;
		trans_val["to"][count * 2 + 1] = itr->second.y;
		++count;
	}
}

void MeshTrans::LoadFromMesh(const Mesh* mesh)
{
	m_map.clear();
	std::set<Node*> unique;
	const std::vector<Triangle*>& tris = mesh->GetTriangles();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		for (int j = 0; j < 3; ++j) 
		{
			Node* node = tris[i]->nodes[j];
			if (unique.find(node) != unique.end()) {
				continue;
			}
			unique.insert(node);
			if (node->xy != node->ori_xy) {
				m_map.insert(std::make_pair(node->ori_xy, node->xy));
			}
		}
	}
}

void MeshTrans::StoreToMesh(Mesh* mesh) const
{
	const std::vector<Triangle*>& tris = mesh->GetTriangles();

	std::set<Node*> unique;
	for (int i = 0, n = tris.size(); i < n; ++i) {
		for (int j = 0; j < 3; ++j) {
			Node* node = tris[i]->nodes[j];
			node->xy = node->ori_xy;
			unique.insert(node);
		}
	}
	for (std::set<Node*>::iterator itr = unique.begin(); itr != unique.end(); ++itr) {
		Node* node = *itr;
		std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr_find 
			= m_map.find(node->ori_xy);
		if (itr_find != m_map.end()) {
			node->xy = itr_find->second;
		}
	}
}

void MeshTrans::SetTween(const MeshTrans& s, const MeshTrans& e, float process)
{
	m_map.clear();
	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr_s 
		= s.m_map.begin();
	for ( ; itr_s != s.m_map.end(); ++itr_s) {
		std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr_e
			= e.m_map.find(itr_s->first);
		sm::vec2 pos;
		if (itr_e == e.m_map.end()) {
			pos = itr_s->second + (itr_s->first - itr_s->second) * process;
		} else {
			pos = itr_s->second + (itr_e->second - itr_s->second) * process;
		}
		m_map.insert(std::make_pair(itr_s->first, pos));
	}
	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr_e 
		= e.m_map.begin();
	for ( ; itr_e != e.m_map.end(); ++itr_e) {
		std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr_s
			= s.m_map.find(itr_e->first);
		if (itr_s == s.m_map.end()) {
			sm::vec2 pos = itr_e->first + (itr_e->second - itr_e->first) * process;
			m_map.insert(std::make_pair(itr_e->first, pos));
		}
	}
}

}