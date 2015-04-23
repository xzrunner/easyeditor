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
{
}

void PathVisibleSimple::DisableRegion(const d2d::ISprite* spr, bool disable)
{
	std::map<const d2d::ISprite*, std::vector<Node*> >::iterator itr 
		= m_bounds.find(spr);
	if (itr == m_bounds.end() && !disable) {
		InsertBoundary(spr);
	} else if (itr != m_bounds.end() && disable) {
		RemoveBoundary(itr);
	}
}

void PathVisibleSimple::QueryRoute(const d2d::Vector& start, const d2d::Vector& end)
{
	
}

void PathVisibleSimple::DebugDraw() const
{
	if (m_bounds.empty()) {
		return;
	}

 	std::map<const d2d::ISprite*, std::vector<Node*> >::const_iterator itr
 		= m_bounds.begin();
	for (int i = 0, n = itr->second.size(); i < n; ++i)
	{
		Node* node = itr->second[i];
		for (int i = 0, n = node->connections.size(); i < n; ++i) {
			const Connection& conn = node->connections[i];
			d2d::PrimitiveDraw::drawLine(node->pos, conn.n->pos, d2d::LIGHT_RED);
		}
	}
}

d2d::Vector PathVisibleSimple::TransIDToPos(int id) const
{
	d2d::Vector ret;
	ret.setInvalid();

	return ret;
}

void PathVisibleSimple::InsertBoundary(const d2d::ISprite* spr)
{
	// get bound
	std::vector<d2d::Vector> bound;
	const libshape::Sprite* shape = dynamic_cast<const libshape::Sprite*>(spr);
	if (shape && shape->getSymbol().GetType() == libshape::e_polygon) {
		const std::vector<d2d::IShape*>& shapes = shape->getSymbol().GetShapes();
		const libshape::PolygonShape* poly = static_cast<const libshape::PolygonShape*>(shapes[0]);
		bound = poly->GetVertices();
	} else {
		spr->getBounding()->getBoundPos(bound);
	}

	// fix
	std::vector<d2d::Vector> fixed;
	d2d::Math::removeDuplicatePoints(bound, fixed);
	d2d::Matrix mat;
	spr->GetTransMatrix(mat);
	for (int i = 0; i < fixed.size(); ++i) {
		fixed[i] = d2d::Math::transVector(fixed[i], mat);
	}

	// create nodes
	std::vector<Node*> nodes;
	for (int i = 0; i < fixed.size(); ++i) {
		Node* node = new Node(m_node_id++, fixed[i]);
		m_nodes.insert(std::make_pair(node->id, node));
		nodes.push_back(node);
	}

	// build connection
	BuildConnection(nodes);

	// insert to visible graph
	m_bounds.insert(std::make_pair(spr, nodes));
}

void PathVisibleSimple::RemoveBoundary(std::map<const d2d::ISprite*, std::vector<Node*> >::iterator itr)
{
	for (int i = 0, n = itr->second.size(); i < n; ++i) 
	{
		Node* node = itr->second[i];

		std::map<int, Node*>::iterator itr_n = m_nodes.find(node->id);
		m_nodes.erase(itr_n);

		std::map<const d2d::ISprite*, std::vector<Node*> >::iterator itr_b
			= m_bounds.begin();
		for ( ; itr_b != m_bounds.end(); ++itr_b)
		{
			if (itr_b == itr) {
				continue;
			}
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

	m_bounds.erase(itr);
}

void PathVisibleSimple::BuildConnection(const std::vector<Node*>& nodes) const
{
	for (int i = 0, n = nodes.size(); i < n; ++i) 
	{
		Node* n0 = nodes[i];
		std::map<const d2d::ISprite*, std::vector<Node*> >::const_iterator itr
			= m_bounds.begin();
		for ( ; itr != m_bounds.end(); ++itr) 
		{
			for (int i = 0, n = itr->second.size(); i < n; ++i)
			{
				Node* n1 = itr->second[i];
				if (!IsSegIntersectAllBound(n0->pos, n1->pos, nodes))
				{
					float dis = d2d::Math::getDistance(n0->pos, n1->pos);
					n0->connections.push_back(Connection(dis, n1));
					n1->connections.push_back(Connection(dis, n0));
				}
			}
		}
	}
}

bool PathVisibleSimple::IsSegIntersectAllBound(const d2d::Vector& p0, const d2d::Vector& p1, 
											   const std::vector<Node*>& bound) const
{
	if (IsSegIntersectBound(p0, p1, bound)) {
		return true;
	}
	std::map<const d2d::ISprite*, std::vector<Node*> >::const_iterator itr
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
	return d2d::Math::IsSegmentIntersectPolyline(p0, p1, points);	
}

}
}