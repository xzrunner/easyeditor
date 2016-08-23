#include "StripMesh.h"
#include "MeshTriangle.h"

#include <SM_Calc.h>

#include <assert.h>

namespace s2
{

StripMesh::StripMesh()
{
}

StripMesh::StripMesh(const StripMesh& strip)
	: Mesh(strip)
	, m_left_nodes(strip.m_left_nodes)
	, m_right_nodes(strip.m_right_nodes)
{
	RefreshTriangles();
	CopyTriangles(strip);
}

StripMesh::StripMesh(const Symbol* base)
	: Mesh(base)
{
	InitBound();
}

void StripMesh::InitBound()
{
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	m_left_nodes.Reset(sm::vec2(-hw, -hh), sm::vec2(-hw,  hh));
	m_right_nodes.Reset(sm::vec2(hw, -hh), sm::vec2( hw,  hh));
}

void StripMesh::RefreshTriangles()
{
	ClearTriangles();

	assert(m_left_nodes.Size() == m_right_nodes.Size());
	const std::vector<sm::vec2>& left = m_left_nodes.m_ext;
	const std::vector<sm::vec2>& right = m_right_nodes.m_ext;
	MeshNode* last_left = new MeshNode(left[0], static_cast<int>(m_width), static_cast<int>(m_height));
	MeshNode* last_right = new MeshNode(right[0], static_cast<int>(m_width), static_cast<int>(m_height));
	for (int i = 0, n = left.size() - 1; i < n; ++i)
	{
		MeshNode* next_left = new MeshNode(left[i+1], static_cast<int>(m_width), static_cast<int>(m_height));
		MeshNode* next_right = new MeshNode(right[i+1], static_cast<int>(m_width), static_cast<int>(m_height));

		MeshTriangle* tri = new MeshTriangle;
		tri->nodes[0] = last_left;
		tri->nodes[1] = last_right;
		tri->nodes[2] = next_right;
		for (int i = 0; i < 3; ++i) {
			tri->nodes[i]->AddReference();
		}
		m_tris.push_back(tri);

		tri = new MeshTriangle;
		tri->nodes[0] = last_left;
		tri->nodes[1] = next_right;
		tri->nodes[2] = next_left;
		for (int i = 0; i < 3; ++i) {
			tri->nodes[i]->AddReference();
		}
		m_tris.push_back(tri);

		last_left = next_left;
		last_right = next_right;
	}
}

void StripMesh::CopyTriangles(const StripMesh& strip)
{
	assert(m_tris.size() == strip.m_tris.size());
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		MeshTriangle* src = strip.m_tris[i];
		MeshTriangle* dst = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			dst->nodes[j]->xy = src->nodes[j]->xy;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// class StripMesh::NodeList
//////////////////////////////////////////////////////////////////////////

void StripMesh::NodeList::
Reset(const sm::vec2& begin, const sm::vec2& end)
{
	m_ori.clear();
	m_ori.push_back(begin);
	m_ori.push_back(end);
	m_ext = m_ori;
}

void StripMesh::NodeList::
Insert(const sm::vec2& p)
{
	Insert(m_ori, p);
	m_ext = m_ori;
}

void StripMesh::NodeList::
Remove(int idx)
{
	m_ori.erase(m_ori.begin() + idx);
	m_ext.erase(m_ext.begin() + idx);
}

int StripMesh::NodeList::
GetNodeInsertPos(const sm::vec2& p, sm::vec2& nearest)
{
	m_ext = m_ori;

	int idx_nearest = -1;
	float dis_nearest = FLT_MAX;
	sm::vec2 foot_nearest;
	for (int i = 0, n = m_ext.size() - 1; i < n; ++i)
	{
		sm::vec2 foot;
		int st = sm::get_foot_of_perpendicular(m_ext[i], m_ext[i+1], p, &foot);
		if (st == -1) {
			continue;
		}
		float dis = sm::dis_square_pos_to_pos(foot, p);
		if (dis < dis_nearest) {
			idx_nearest = i;
			dis_nearest = dis;
			foot_nearest = foot;
		}
	}

	if (idx_nearest != -1) {
		nearest = foot_nearest;
	}

	return idx_nearest;
}

int StripMesh::NodeList::
QueryIndex(const sm::vec2& p, float radius) const
{
	// front and back are bound
	for (int i = 1, n = m_ori.size() - 1; i < n; ++i) {
		if (sm::dis_pos_to_pos(m_ori[i], p) < radius) {
			return i;
		}
	}
	return -1;
}

sm::vec2* StripMesh::NodeList::
QueryPointer(const sm::vec2& p, float radius)
{
	int idx = QueryIndex(p, radius);
	if (idx != -1) {
		return &m_ori[idx];
	} else {
		return NULL;
	}
}

bool StripMesh::NodeList::
IsRegionContain(const sm::vec2& p) const
{
	sm::vec2 foot;
	return sm::get_foot_of_perpendicular(m_ori.front(), m_ori.back(), p, &foot) != -1;
}

void StripMesh::NodeList::
Sort()
{
	std::sort(m_ori.begin(), m_ori.end(), sm::Vector2CmpY());
	std::sort(m_ext.begin(), m_ext.end(), sm::Vector2CmpY());
}

void StripMesh::NodeList::
Insert(std::vector<sm::vec2>& nodes, const sm::vec2& p)
{
	int idx_nearest = -1;
	float dis_nearest = FLT_MAX;
	sm::vec2 foot_nearest;
	for (int i = 0, n = nodes.size() - 1; i < n; ++i)
	{
		sm::vec2 foot;
		int st = sm::get_foot_of_perpendicular(nodes[i], nodes[i+1], p, &foot);
		if (st == -1) {
			continue;
		}
		float dis = sm::dis_square_pos_to_pos(foot, p);
		if (dis < dis_nearest) {
			idx_nearest = i;
			dis_nearest = dis;
			foot_nearest = foot;
		}
	}
	assert(idx_nearest != -1);
	nodes.insert(nodes.begin() + idx_nearest + 1, foot_nearest);
}

}