#include "PathVisibleSimple.h"

#include <easyshape.h>

namespace lr
{
namespace preview
{

PathVisibleSimple::PathVisibleSimple(const d2d::Rect& region)
	: m_region(region)
	, m_visited(this)
	, m_node_id(0)
	, m_last_start(NULL)
	, m_last_end(NULL)
{
}

void PathVisibleSimple::DisableRegion(const d2d::Sprite* spr, bool disable)
{
	std::map<const d2d::Sprite*, std::vector<Node*> >::iterator itr 
		= m_bounds.find(spr);
	if (itr == m_bounds.end() && !disable) {
		InsertBoundary(spr);
	} else if (itr != m_bounds.end() && disable) {
		RemoveBoundary(itr);
	}
}

void PathVisibleSimple::QueryRoute(const d2d::Vector& start, const d2d::Vector& end)
{
	VisitedNode* node = QueryRouteImpl(start, end);
	if (!node) {
		return;
	}

// 	float dx = -m_region.xLength() * 0.5f,
// 		dy = -m_region.yLength() * 0.5f;
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

 	std::map<const d2d::Sprite*, std::vector<Node*> >::const_iterator itr
 		= m_bounds.begin();
	for ( ; itr != m_bounds.end(); ++itr)
	{
		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
			Node* node = itr->second[i];
			for (int j = 0, m = node->connections.size(); j < m; ++j) {
				const Connection& conn = node->connections[j];
				d2d::PrimitiveDraw::DrawLine(node->pos, conn.n->pos, d2d::BLACK);
			}
		}
	}

	d2d::PrimitiveDraw::DrawPolyline(m_routes, d2d::SELECT_RED, false);
}

d2d::Vector PathVisibleSimple::TransIDToPos(int id) const
{
	std::map<int, Node*>::const_iterator itr = m_nodes.find(id);
	if (itr != m_nodes.end()) {
		return itr->second->pos;
	} else {
		d2d::Vector ret;
		ret.SetInvalid();
		return ret;
	}
}

void PathVisibleSimple::InsertBoundary(const d2d::Sprite* spr)
{
	// get bound
	std::vector<d2d::Vector> bound;
	const libshape::Sprite* shape = dynamic_cast<const libshape::Sprite*>(spr);
	if (shape && shape->GetSymbol().GetShapeType() == libshape::ST_POLYGON) {
		const std::vector<d2d::Shape*>& shapes = shape->GetSymbol().GetShapes();
		const libshape::PolygonShape* poly = static_cast<const libshape::PolygonShape*>(shapes[0]);
		bound = poly->GetVertices();
	} else {
		spr->GetBounding()->GetBoundPos(bound);
	}

	// fix
	std::vector<d2d::Vector> fixed;
	d2d::Math2D::RemoveDuplicatePoints(bound, fixed);
	d2d::Matrix mat;
	spr->GetTransMatrix(mat);
	for (int i = 0; i < fixed.size(); ++i) {
		fixed[i] = d2d::Math2D::TransVector(fixed[i], mat);
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

void PathVisibleSimple::RemoveBoundary(std::map<const d2d::Sprite*, std::vector<Node*> >::iterator itr)
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
		std::map<const d2d::Sprite*, std::vector<Node*> >::const_iterator itr
			= m_bounds.begin();
		for ( ; itr != m_bounds.end(); ++itr) 
		{
			for (int i = 0, n = itr->second.size(); i < n; ++i)
			{
				Node* n1 = itr->second[i];
				if (!IsSegIntersectAllBound(n0->pos, n1->pos))
				{
					float dis = d2d::Math2D::GetDistance(n0->pos, n1->pos);
					n0->connections.push_back(Connection(dis, n1));
					n1->connections.push_back(Connection(dis, n0));
				}
			}
		}
	}
}

bool PathVisibleSimple::IsSegIntersectAllBound(const d2d::Vector& p0, const d2d::Vector& p1) const
{
	std::map<const d2d::Sprite*, std::vector<Node*> >::const_iterator itr
		= m_bounds.begin();
	for ( ; itr != m_bounds.end(); ++itr) {
		if (IsSegIntersectBound(p0, p1, itr->second)) {
			return true;
		}
	}
	return false;
}

bool PathVisibleSimple::IsSegIntersectBound(const d2d::Vector& p0, const d2d::Vector& p1, 
											const std::vector<Node*>& bound) const
{
	std::vector<d2d::Vector> points;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		points.push_back(bound[i]->pos);
	}
	if (d2d::Math2D::IsPointInArea((p0+p1)*0.5f, points)) {
		return true;
	}
	return d2d::Math2D::IsSegmentIntersectPolyline(p0, p1, points);	
}

PathVisibleSimple::Node* PathVisibleSimple::CreateNode(const d2d::Vector& pos)
{
	Node* n0 = new Node(m_node_id++, pos);
	m_nodes.insert(std::make_pair(n0->id, n0));

	std::map<const d2d::Sprite*, std::vector<Node*> >::const_iterator itr
		= m_bounds.begin();
	for ( ; itr != m_bounds.end(); ++itr) 
	{
		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
			Node* n1 = itr->second[i];
			if (!IsSegIntersectAllBound(n0->pos, n1->pos))
			{
				float dis = d2d::Math2D::GetDistance(n0->pos, n1->pos);
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

	std::map<const d2d::Sprite*, std::vector<Node*> >::iterator itr_b
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

VisitedNode* PathVisibleSimple::QueryRouteImpl(const d2d::Vector& start, const d2d::Vector& end)
{
	if (!d2d::Math2D::IsPointInRect(start, m_region) || !d2d::Math2D::IsPointInRect(end, m_region)) {
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

void PathVisibleSimple::Expand(VisitedNode* node, const d2d::Vector& end)
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
			float to = d2d::Math2D::GetDistance(end, TransIDToPos(ct.n->id));
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