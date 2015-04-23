#ifndef _LR_PREVIEW_PATH_VISIBLE_SIMPLE_H_
#define _LR_PREVIEW_PATH_VISIBLE_SIMPLE_H_

#include "IPathfinding.h"
#include "PathUtil.h"

#include <drag2d.h>

namespace lr
{
namespace preview
{

class PathVisibleSimple : public IPathfinding, public INetwork
{
public:
	PathVisibleSimple(const d2d::Rect& region);

	//
	// interface IPathfinding
	//
	virtual void DisableRegion(const d2d::ISprite* spr, bool disable);
	virtual void QueryRoute(const d2d::Vector& start, const d2d::Vector& end);
	virtual void DebugDraw() const;

	//
	// interface INetwork
	//
	virtual d2d::Vector TransIDToPos(int id) const;

private:
	struct Node;
	struct Connection
	{
		Connection(float len, Node* n) 
			: len(len), n(n) {}

		float len;

		Node* n;
	}; // Connection

	struct Node
	{
		Node(int id, const d2d::Vector& pos)
			: id(id), pos(pos) {}

		int id;

		d2d::Vector pos;

		std::vector<Connection> connections;

	}; // Node

private:
	void InsertBoundary(const d2d::ISprite* spr);
	void RemoveBoundary(std::map<const d2d::ISprite*, std::vector<Node*> >::iterator itr);

	void BuildConnection(const std::vector<Node*>& nodes) const;
	bool IsSegIntersectBound(const d2d::Vector& p0, const d2d::Vector& p1, 
		const std::vector<Node*>& bound) const;

private:
	d2d::Rect m_region;

	VisitedList m_visited;
	CandidateList m_candidate;

	std::map<const d2d::ISprite*, std::vector<Node*> > m_bounds;
	
	int m_node_id;
	std::map<int, Node*> m_nodes;

}; // PathVisibleSimple

}
}

#endif // _LR_PREVIEW_PATH_VISIBLE_SIMPLE_H_