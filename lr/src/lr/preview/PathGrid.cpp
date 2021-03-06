#include "PathGrid.h"

#include <ee/Sprite.h>
#include <ee/color_config.h>

#include <sprite2/RVG.h>
#include <SM_Test.h>
#include <SM_Calc.h>

namespace lr
{
namespace preview
{

PathGrid::PathGrid(const sm::rect& region, int row, int col)
	: m_nw(region, row, col)
{
}

void PathGrid::DisableRegion(const ee::SprConstPtr& spr, bool disable)
{
	m_nw.SetStatus(spr->GetBounding().GetSize(), !disable);
}

void PathGrid::QueryRoute(const sm::vec2& start, const sm::vec2& end)
{
	VisitedNode* node = m_nw.QueryRoute(start, end);
	if (!node) {
		return;
	}

	sm::vec2 sz = m_nw.m_region.Size();
	float dx = -sz.x * 0.5f,
		  dy = -sz.y * 0.5f;
	m_routes.clear();
	while (node) {
		int y = node->m_id / m_nw.m_col;
		int x = node->m_id - y * m_nw.m_col;
		m_routes.push_back(sm::vec2((x+0.5f)*m_nw.m_width+dx, (y+0.5f)*m_nw.m_height+dy));
		node = node->m_prev;
	}
}

void PathGrid::DebugDraw() const
{
	m_nw.DebugDraw();	
	s2::RVG::SetColor(ee::LIGHT_RED);
	s2::RVG::Polyline(m_routes, false);
}

//////////////////////////////////////////////////////////////////////////
// class PathGrid::Network
//////////////////////////////////////////////////////////////////////////

PathGrid::Network::
Network(const sm::rect& region, int row, int col)
	: m_region(region)
	, m_row(row)
	, m_col(col)
	, m_visited(this)
{
	sm::vec2 r_sz = m_region.Size();
	m_width = r_sz.x / m_col;
	m_height = r_sz.y / m_row;

	int sz = m_row * m_col;
	m_nodes = new Node[sz];
	for (int i = 0; i < sz; ++i) {
		m_nodes[i].id = i;
	}
}

PathGrid::Network::
~Network()
{
	delete[] m_nodes;
}

sm::vec2 PathGrid::Network::
TransIDToPos(int id) const
{
	sm::vec2 ret;

	int y = id / m_col;
	int x = id - y * m_col;

	if (x <= 0 || x >= m_col-1 || y <= 0 || y >= m_row-1) {
		return ret;
	}

	sm::vec2 sz = m_region.Size();
	ret.x = m_width * x - sz.x * 0.5f;
	ret.y = m_height * y - sz.y * 0.5f;

	return ret;
}

void PathGrid::Network::
SetStatus(const sm::rect& region, bool used)
{
	if (!sm::is_rect_contain_rect(m_region, region)) {
		return;
	}

	int xs = floor((region.xmin - m_region.xmin) / m_width),
		xe = ceil((region.xmax - m_region.xmin) / m_width);
	int ys = floor((region.ymin - m_region.ymin) / m_height),
		ye = ceil((region.ymax - m_region.ymin) / m_height);
	for (int y = ys; y < ye; ++y) {
		for (int x = xs; x < xe; ++x) {
			m_nodes[y * m_col + x].m_used = used;
		}
	}
}

VisitedNode* PathGrid::Network::
QueryRoute(const sm::vec2& start, const sm::vec2& end)
{
	if (!sm::is_point_in_rect(start, m_region) || !sm::is_point_in_rect(end, m_region)) {
		return NULL;
	}

	m_visited.Clear();
	m_candidate.Clear();

	Node *sn = QueryNode(start),
		*en = QueryNode(end);
	VisitedNode* sv = new VisitedNode(sn->id);
	m_visited.Push(sv);
	m_candidate.Push(sv);
	while (!m_candidate.IsEmpty()) 
	{
		VisitedNode* opt = m_candidate.Top(); m_candidate.Pop();
		if (opt->m_id == en->id) {
			return opt;
		} else {
			Expand(opt, end);
		}
	}

	return NULL;
}

void PathGrid::Network::
DebugDraw() const
{
	sm::vec2 sz = m_region.Size();
	float dx = -sz.x * 0.5f,
		  dy = -sz.y * 0.5f;
	int idx = 0;
	for (int y = 0; y < m_row; ++y) {
		for (int x = 0; x < m_col; ++x) {
			if (!m_nodes[idx++].m_used) {
				continue;
			}
			sm::vec2 p0(x * m_width + dx, y * m_height + dy);
			sm::vec2 p1(p0.x + m_width, p0.y + m_height);
			s2::RVG::SetColor(ee::LIGHT_GREEN);
			s2::RVG::Rect(p0, p1, true);
		}
	}

	m_visited.DebugDraw();
}

PathGrid::Node* PathGrid::Network::
QueryNode(const sm::vec2& pos) const
{
	if (!sm::is_point_in_rect(pos, m_region)) {
		return NULL;
	}

	sm::vec2 sz = m_region.Size();
	float dx = sz.x * 0.5f,
		  dy = sz.y * 0.5f;
	int x = floor((pos.x+dx) / m_width),
		y = floor((pos.y+dy) / m_height);
	return &m_nodes[y * m_col + x];
}

void PathGrid::Network::
Expand(VisitedNode* node, const sm::vec2& end)
{
	std::vector<Connection> connections;
	GetConnections(node, connections);
	for (int i = 0, n = connections.size(); i < n; ++i)
	{
		const Connection& ct = connections[i];
		if (ct.n->m_used) {
			continue;
		}

		VisitedNode* visited = m_visited.Find(ct.n->id);
		if (visited)
		{
			float new_from = node->m_from + ct.len;
			if (new_from < visited->m_from) {
				m_candidate.Update(visited, new_from, node);
			}
		}
		else
		{
			float to = sm::dis_pos_to_pos(end, TransIDToPos(ct.n->id));
			VisitedNode* new_node = new VisitedNode(ct.n->id, node, node->m_from + ct.len, to);
			m_visited.Push(new_node);
			m_candidate.Push(new_node);
		}
	}
}

void PathGrid::Network::
GetConnections(VisitedNode* node, std::vector<Connection>& connections) const
{
	int y = node->m_id / m_col;
	int x = node->m_id - y * m_col;

	if (x <= 0 || x >= m_col-1 || y <= 0 || y >= m_row-1) {
		return;
	}

	int idx = y * m_col + x;

	connections.push_back(Connection(m_width, &m_nodes[idx-1]));
	connections.push_back(Connection(m_width, &m_nodes[idx+1]));
	connections.push_back(Connection(m_height, &m_nodes[idx-m_col]));
	connections.push_back(Connection(m_height, &m_nodes[idx+m_col]));

	float diagonal = sqrt(m_width*m_width+m_height*m_height);
	connections.push_back(Connection(diagonal, &m_nodes[idx-m_col-1]));
	connections.push_back(Connection(diagonal, &m_nodes[idx-m_col+1]));
	connections.push_back(Connection(diagonal, &m_nodes[idx+m_col-1]));
	connections.push_back(Connection(diagonal, &m_nodes[idx+m_col+1]));
}

}
}