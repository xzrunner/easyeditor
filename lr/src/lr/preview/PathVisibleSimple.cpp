#include "PathVisibleSimple.h"

#include <ee/color_config.h>
#include <ee/Math2D.h>

#include <easyshape.h>

#include <sprite2/S2_RVG.h>

namespace lr
{
namespace preview
{

PathVisibleSimple::PathVisibleSimple(const sm::rect& region)
	: m_region(region)
	, m_visited(this)
	, m_node_id(0)
	, m_last_start(NULL)
	, m_last_end(NULL)
{
}

void PathVisibleSimple::DisableRegion(const ee::Sprite* spr, bool disable)
{
	std::map<const ee::Sprite*, std::vector<Node*> >::iterator itr 
		= m_bounds.find(spr);
	if (itr == m_bounds.end() && !disable) {
		InsertBoundary(spr);
	} else if (itr != m_bounds.end() && disable) {
		RemoveBoundary(itr);
	}
}

void PathVisibleSimple::QueryRoute(const sm::vec2& start, const sm::vec2& end)
{
	VisitedNode* node = QueryRouteImpl(start, end);
	if (!node) {
		return;
	}

// 	float dx = -m_region.Width() * 0.5f,
// 		dy = -m_region.Height() * 0.5f;
	m_routes.clear();
	while (node) {		
		m_routes.push_back(TransIDToPos(node->m_id));
		node = node->m_prev;
	}
}

void PathVisibleSimple::DebugDraw() const
{
	if (m_bounds.empty()) {
		return;
	}

 	std::map<const ee::Sprite*, std::vector<Node*> >::const_iterator itr
 		= m_bounds.begin();
	for ( ; itr != m_bounds.end(); ++itr)
	{
		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
			Node* node = itr->second[i];
			for (int j = 0, m = node->connections.size(); j < m; ++j) {
				const Connection& conn = node->connections[j];
				s2::RVG::SetColor(ee::BLACK);
				s2::RVG::Line(node->pos, conn.n->pos);
			}
		}
	}

	s2::RVG::SetColor(ee::SELECT_RED);
	s2::RVG::Polyline(m_routes, false);
}

sm::vec2 PathVisibleSimple::TransIDToPos(int id) const
{
	std::map<int, Node*>::const_iterator itr = m_nodes.find(id);
	if (itr != m_nodes.end()) {
		return itr->second->pos;
	} else {
		return sm::vec2();
	}
}

void PathVisibleSimple::InsertBoundary(const ee::Sprite* spr)
{
	// get bound
	std::vector<sm::vec2> bound;
	const eshape::Sprite* shape = dynamic_cast<const eshape::Sprite*>(spr);
	if (shape && shape->GetSymbol().GetShapeType() == eshape::ST_POLYGON) {
		const std::vector<ee::Shape*>& shapes = shape->GetSymbol().GetShapes();
		const eshape::PolygonShape* poly = static_cast<const eshape::PolygonShape*>(shapes[0]);
		bound = poly->GetVertices();
	} else {
		spr->GetBounding()->GetBoundPos(bound);
	}

	// fix
	std::vector<sm::vec2> fixed;
	ee::Math2D::RemoveDuplicatePoints(bound, fixed);
	sm::mat4 mat = spr->GetTransMatrix();
	for (int i = 0; i < fixed.size(); ++i) {
		fixed[i] = ee::Math2D::TransVector(fixed[i], mat);
	}

	// create nodes
	std::vector<Node*> nodes;
	for (int i = 0; i < fixed.size(); ++i) {
		Node* node = new Node(m_node_id++, fixed[i]);
		m_nodes.insert(std::make_pair(node->id, node));
		nodes.push_back(node);
	}

	// insert to visible graph
	m_bounds.insert(std::make_pair(spr, nodes));

	// build connection
	BuildConnection(nodes);
}

void PathVisibleSimple::RemoveBoundary(std::map<const ee::Sprite*, std::vector<Node*> >::iterator itr)
{
	for (int i = 0, n = itr->second.size(); i < n; ++i) {
		RemoveNode(itr->second[i]);
	}

	m_bounds.erase(itr);
}

void PathVisibleSimple::BuildConnection(const std::vector<Node*>& nodes) const
{
	for (int i = 0, n = nodes.size(); i < n; ++i) 
	{
		Node* n0 = nodes[i];
		std::map<const ee::Sprite*, std::vector<Node*> >::const_iterator itr
			= m_bounds.begin();
		for ( ; itr != m_bounds.end(); ++itr) 
		{
			for (int i = 0, n = itr->second.size(); i < n; ++i)
			{
				Node* n1 = itr->second[i];
				if (!IsSegIntersectAllBound(n0->pos, n1->pos))
				{
					float dis = ee::Math2D::GetDistance(n0->pos, n1->pos);
					n0->connections.push_back(Connection(dis, n1));
					n1->connections.push_back(Connection(dis, n0));
				}
			}
		}
	}
}

bool PathVisibleSimple::IsSegIntersectAllBound(const sm::vec2& p0, const sm::vec2& p1) const
{
	std::map<const ee::Sprite*, std::vector<Node*> >::const_iterator itr
		= m_bounds.begin();
	for ( ; itr != m_bounds.end(); ++itr) {
		if (IsSegIntersectBound(p0, p1, itr->second)) {
			return true;
		}
	}
	return false;
}

bool PathVisibleSimple::IsSegIntersectBound(const sm::vec2& p0, const sm::vec2& p1, 
											const std::vector<Node*>& bound) const
{
	std::vector<sm::vec2> points;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		points.push_back(bound[i]->pos);
	}
	if (ee::Math2D::IsPointInArea((p0+p1)*0.5f, points)) {
		return true;
	}
	return ee::Math2D::IsSegmentIntersectPolyline(p0, p1, points);	
}

PathVisibleSimple::Node* PathVisibleSimple::CreateNode(const sm::vec2& pos)
{
	Node* n0 = new Node(m_node_id++, pos);
	m_nodes.insert(std::make_pair(n0->id, n0));

	std::map<const ee::Sprite*, std::vector<Node*> >::const_iterator itr
		= m_bounds.begin();
	for ( ; itr != m_bounds.end(); ++itr) 
	{
		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
			Node* n1 = itr->second[i];
			if (!IsSegIntersectAllBound(n0->pos, n1->pos))
			{
				float dis = ee::Math2D::GetDistance(n0->pos, n1->pos);
				n0->connections.push_back(Connection(dis, n1));
				n1->connections.push_back(Connection(dis, n0));
			}
		}
	}

	return n0;
}

void PathVisibleSimple::RemoveNode(const Node* node)
{
	std::map<int, Node*>::iterator itr_n = m_nodes.find(node->id);
	m_nodes.erase(itr_n);

	std::map<const ee::Sprite*, std::vector<Node*> >::iterator itr_b
		= m_bounds.begin();
	for ( ; itr_b != m_bounds.end(); ++itr_b)
	{
		for (int i = 0, n = itr_b->second.size(); i < n; ++i) {
			Node* node2 = itr_b->second[i];
			std::vector<Connection>::iterator itr_c = node2->connections.begin();
			for ( ; itr_c != node2->connections.end(); ) {
				if (itr_c->n == node) {
					itr_c = node2->connections.erase(itr_c);
				} else {
					++itr_c;
				}
			}
		}
	}

	delete node;
}

VisitedNode* PathVisibleSimple::QueryRouteImpl(const sm::vec2& start, const sm::vec2& end)
{
	if (!ee::Math2D::IsPointInRect(start, m_region) || !ee::Math2D::IsPointInRect(end, m_region)) {
		return NULL;
	}

	if (m_last_start) {
		RemoveNode(m_last_start);
	}
	if (m_last_end) {
		RemoveNode(m_last_end);
	}

	m_visited.Clear();
	m_candidate.Clear();

	Node *sn = CreateNode(start),
		*en = CreateNode(end);
	m_last_start = sn;
	m_last_end = en;

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

void PathVisibleSimple::Expand(VisitedNode* node, const sm::vec2& end)
{
	std::vector<Connection> connections;
	GetConnections(node, connections);
	for (int i = 0, n = connections.size(); i < n; ++i)
	{
		const Connection& ct = connections[i];

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
			float to = ee::Math2D::GetDistance(end, TransIDToPos(ct.n->id));
			VisitedNode* new_node = new VisitedNode(ct.n->id, node, node->m_from + ct.len, to);
			m_visited.Push(new_node);
			m_candidate.Push(new_node);
		}
	}
}

void PathVisibleSimple::GetConnections(VisitedNode* node, std::vector<Connection>& connections) const
{
	std::map<int, Node*>::const_iterator itr = m_nodes.find(node->m_id);
	assert(itr != m_nodes.end());

	Node* n = itr->second;
	connections = n->connections;
}

}
}