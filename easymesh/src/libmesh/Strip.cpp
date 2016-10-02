#include "Strip.h"

#include <ee/Math2D.h>
#include <ee/Symbol.h>

#include <SM_Calc.h>
#include <sprite2/MeshTriangle.h>
#include <gum/JsonSerializer.h>

#include <set>
#include <algorithm>

#include <assert.h>
#include <math.h>

namespace emesh
{

Strip::Strip()
	: m_uv_offset(0)
{
}

Strip::Strip(const Strip& strip)
	: s2::Mesh(strip)
	, Mesh(strip)
	, s2::StripMesh(strip)
{
}

Strip::Strip(const ee::Symbol* base)
	: s2::Mesh(base)
	, Mesh(base)
	, s2::StripMesh(base)
{
}

//Strip* Strip::Clone() const
//{
//	return new Strip(*this);
//}

void Strip::Load(const Json::Value& value)
{
	m_width  = static_cast<float>(value["width"].asDouble());
	m_height = static_cast<float>(value["height"].asDouble());

	m_left_nodes.m_ori.clear();
	m_right_nodes.m_ori.clear();
	gum::JsonSerializer::Load(value["left nodes"], m_left_nodes.m_ori);
	gum::JsonSerializer::Load(value["right nodes"], m_right_nodes.m_ori);
	m_left_nodes.m_ext = m_left_nodes.m_ori;
	m_right_nodes.m_ext = m_right_nodes.m_ori;

	RefreshTriangles();

	LoadTriangles(value["triangles"]);
}

void Strip::Store(Json::Value& value) const
{
	value["type"] = GetTypeName();

	value["width"] = m_width;
	value["height"] = m_height;

	gum::JsonSerializer::Store(m_left_nodes.m_ori, value["left nodes"]);
	gum::JsonSerializer::Store(m_right_nodes.m_ori, value["right nodes"]);

	StoreTriangles(value["triangles"]);
}

void Strip::OffsetUV(float dx, float dy)
{
	// update uv base
	m_uv_offset += dy;
	m_uv_offset = m_uv_offset - floor(m_uv_offset);

	std::vector<std::pair<sm::vec2, sm::vec2> > trans_list;
	GetTransList(trans_list);

	// insert node
	sm::vec2 pos;
	pos.x = 0;
	pos.y = -m_height*0.5f + m_height*m_uv_offset;
	int idx_left, idx_right;
	sm::vec2 pos_left, pos_right;
	idx_left = m_left_nodes.GetNodeInsertPos(pos, pos_left);
	idx_right = m_right_nodes.GetNodeInsertPos(pos, pos_right);
	if (idx_left != -1 && idx_right != -1) {
		std::vector<sm::vec2>& left_ext = m_left_nodes.m_ext;
		std::vector<sm::vec2>& right_ext = m_right_nodes.m_ext;
		MapUV2XY(left_ext, idx_left, pos, trans_list);
		MapUV2XY(right_ext, idx_right, pos, trans_list);
		left_ext.insert(left_ext.begin() + idx_left + 1, pos_left);
		right_ext.insert(right_ext.begin() + idx_right + 1, pos_right);
	}

	// create triangles separate
	ClearTriangles();
	assert(m_left_nodes.Size() == m_right_nodes.Size());
	const std::vector<sm::vec2>& left = m_left_nodes.m_ext;
	const std::vector<sm::vec2>& right = m_right_nodes.m_ext;
	for (int i = 0, n = left.size() - 1; i < n; ++i)
	{
		if (left[i] == left[i+1] && right[i] == right[i+1]) {
			continue;
		}

		s2::MeshTriangle* tri = new s2::MeshTriangle;
		tri->nodes[0] = new s2::MeshNode(left[i], static_cast<int>(m_width), static_cast<int>(m_height));
		tri->nodes[1] = new s2::MeshNode(right[i], static_cast<int>(m_width), static_cast<int>(m_height));
		tri->nodes[2] = new s2::MeshNode(right[i+1], static_cast<int>(m_width), static_cast<int>(m_height));
		m_tris.push_back(tri);

		tri = new s2::MeshTriangle;
		tri->nodes[0] = new s2::MeshNode(left[i], static_cast<int>(m_width), static_cast<int>(m_height));
		tri->nodes[1] = new s2::MeshNode(right[i+1], static_cast<int>(m_width), static_cast<int>(m_height));
		tri->nodes[2] = new s2::MeshNode(left[i+1], static_cast<int>(m_width), static_cast<int>(m_height));
		m_tris.push_back(tri);
	}
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		for (int j = 0; j < 3; ++j) {
			TranslateNode(m_tris[i]->nodes[j], trans_list);
		}
	}

	// set uv between textures
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		s2::MeshTriangle* tri = m_tris[i];
		sm::rect r;
		r.MakeEmpty();
		for (int i = 0; i < 3; ++i) {
			r.Combine(tri->nodes[i]->uv);
		}

		for (int i = 0; i < 3; ++i) 
		{
			s2::MeshNode* n = tri->nodes[i];
			float y = n->uv.y - m_uv_offset;
			y = y - floor(y);
			if (fabs(y - 0) < 0.0001f && n->uv.y == r.ymax) {
				y = 1;
			}
			if (fabs(y - 1) < 0.0001f && n->uv.y == r.ymin) {
				y = 0;
			}
			n->uv.y = y;
		}
	}
}

void Strip::Reset()
{
	m_uv_offset = 0;
	RefreshTriangles();
}

void Strip::Clear()
{
	InitBound();
	RefreshTriangles();
}

void Strip::InsertNode(const sm::vec2& p)
{
	assert(m_left_nodes.Size() >= 2 && m_right_nodes.Size() >= 2);

	if (!m_left_nodes.IsRegionContain(p) || !m_right_nodes.IsRegionContain(p)) {
		return;
	}

	m_left_nodes.Insert(p);
	m_right_nodes.Insert(p);

	RefreshTriangles();
}

void Strip::RemoveNode(const sm::vec2& p)
{
	assert(m_left_nodes.Size() >= 2 && m_right_nodes.Size() >= 2);

	int idx;
	idx = m_left_nodes.QueryIndex(p, m_node_radius);
	if (idx != -1) 
	{
		m_left_nodes.Remove(idx);
		m_right_nodes.Remove(idx);
		RefreshTriangles();
	} 
	else 
	{
		idx = m_right_nodes.QueryIndex(p, m_node_radius);
		if (idx != -1) 
		{
			m_left_nodes.Remove(idx);
			m_right_nodes.Remove(idx);
			RefreshTriangles();
		}
	}
}

sm::vec2* Strip::FindNode(const sm::vec2& p)
{
	assert(m_left_nodes.Size() >= 2 && m_right_nodes.Size() >= 2);

	sm::vec2* ptr = m_left_nodes.QueryPointer(p, m_node_radius);
	if (ptr == NULL) {
		ptr = m_right_nodes.QueryPointer(p, m_node_radius);
	}
	return ptr;
}

void Strip::MoveNode(sm::vec2* src, const sm::vec2& dst)
{
	assert(m_left_nodes.Size() >= 2 && m_right_nodes.Size() >= 2);

	if (!src) return;

	src->y = dst.y;

	m_left_nodes.Sort();
	m_right_nodes.Sort();

	RefreshTriangles();
}

void Strip::AbsorbNodeToRegion(sm::vec2& node)
{
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	if (fabs(node.x - (-hw)) < m_node_radius) {
		node.x = -hw;
	}
	if (fabs(node.x - hw) < m_node_radius) {
		node.x = hw;
	}
	if (fabs(node.y - (-hh)) < m_node_radius) {
		node.y = -hh;
	}
	if (fabs(node.y - hh) < m_node_radius) {
		node.y = hh;
	}
}

void Strip::GetTransList(std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list) const
{
	std::set<s2::MeshNode*> nodes;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		s2::MeshTriangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i) {
			nodes.insert(tri->nodes[i]);
		}
	}
	std::set<s2::MeshNode*>::iterator itr = nodes.begin();
	for ( ; itr != nodes.end(); ++itr)
	{
		s2::MeshNode* n = *itr;
		if (n->xy != n->ori_xy) {
			trans_list.push_back(std::make_pair(n->ori_xy, n->xy));
		}
	}
}

void Strip::TranslateNode(s2::MeshNode* node, const std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list)
{
	for (int i = 0, m = trans_list.size(); i < m; ++i)
	{
		float dis = sm::dis_square_pos_to_pos(node->ori_xy, trans_list[i].first);
		if (dis < 0.01) {
			node->xy = trans_list[i].second;
			break;
		}
	}
}

void Strip::TranslateNode(sm::vec2& node, const std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list)
{
	for (int i = 0, m = trans_list.size(); i < m; ++i)
	{
		float dis = sm::dis_square_pos_to_pos(node, trans_list[i].first);
		if (dis < 0.01) {
			node = trans_list[i].second;
			break;
		}
	}
}

void Strip::MapUV2XY(const std::vector<sm::vec2>& nodes, int index, const sm::vec2& pos, 
					 std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list)
{
	const sm::vec2& s = nodes[index];
	const sm::vec2& e = nodes[index+1];

	sm::vec2 xy_s = s, xy_e = e;
	TranslateNode(xy_s, trans_list);
	TranslateNode(xy_e, trans_list);

	sm::vec2 foot;
	int st = sm::get_foot_of_perpendicular(s, e, pos, &foot);
	
	float p = sm::dis_pos_to_pos(s, foot) / sm::dis_pos_to_pos(s, e);
 	sm::vec2 xy_p = xy_s + (xy_e - xy_s) * p;
 	trans_list.push_back(std::make_pair(foot, xy_p));
}

}