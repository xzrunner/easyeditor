#include "MeshTransform.h"
#include "MeshTriangle.h"
#include "Mesh.h"

#include <set>
#include <vector>

namespace s2
{

void MeshTransform::LoadFromMesh(const Mesh* mesh)
{
	m_map.clear();
	std::set<MeshNode*> unique;
	const std::vector<MeshTriangle*>& tris = mesh->GetTriangles();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		for (int j = 0; j < 3; ++j) 
		{
			MeshNode* node = tris[i]->nodes[j];
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

void MeshTransform::StoreToMesh(Mesh* mesh) const
{
	const std::vector<MeshTriangle*>& tris = mesh->GetTriangles();

	std::set<MeshNode*> unique;
	for (int i = 0, n = tris.size(); i < n; ++i) {
		for (int j = 0; j < 3; ++j) {
			MeshNode* node = tris[i]->nodes[j];
			node->xy = node->ori_xy;
			unique.insert(node);
		}
	}
	for (std::set<MeshNode*>::iterator itr = unique.begin(); itr != unique.end(); ++itr) {
		MeshNode* node = *itr;
		std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr_find 
			= m_map.find(node->ori_xy);
		if (itr_find != m_map.end()) {
			node->xy = itr_find->second;
		}
	}
}

void MeshTransform::SetTween(const MeshTransform& s, const MeshTransform& e, float process)
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