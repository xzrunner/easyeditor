#ifndef _LR_PREVIEW_PATH_VISIBLE_SIMPLE_H_
#define _LR_PREVIEW_PATH_VISIBLE_SIMPLE_H_

#include "IPathfinding.h"
#include "PathUtil.h"

#include <ee/Rect.h>
 
#include <vector>
#include <map>

namespace lr
{
namespace preview
{

class PathVisibleSimple : public IPathfinding, public INetwork
{
public:
	PathVisibleSimple(const ee::Rect& region);

	//
	// interface IPathfinding
	//
	virtual void DisableRegion(const ee::Sprite* spr, bool disable);
	virtual void QueryRoute(const ee::Vector& start, const ee::Vector& end);
	virtual void DebugDraw() const;

	//
	// interface INetwork
	//
	virtual ee::Vector TransIDToPos(int id) const;

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
		Node(int id, const ee::Vector& pos)
			: id(id), pos(pos) {}

		int id;

		ee::Vector pos;

		std::vector<Connection> connections;

	}; // Node

private:
	// data struct
	void InsertBoundary(const ee::Sprite* spr);
	void RemoveBoundary(std::map<const ee::Sprite*, std::vector<Node*> >::iterator itr);

	void BuildConnection(const std::vector<Node*>& nodes) const;
	bool IsSegIntersectAllBound(const ee::Vector& p0, const ee::Vector& p1) const;
	bool IsSegIntersectBound(const ee::Vector& p0, const ee::Vector& p1, 
		const std::vector<Node*>& bound) const;

	Node* CreateNode(const ee::Vector& pos);
	void RemoveNode(const Node* node);

	// route
	VisitedNode* QueryRouteImpl(const ee::Vector& start, const ee::Vector& end);
	void Expand(VisitedNode* node, const ee::Vector& end);
	void GetConnections(VisitedNode* node, std::vector<Connection>& connections) const;

private:
	ee::Rect m_region;

	VisitedList m_visited;
	CandidateList m_candidate;

	std::map<const ee::Sprite*, std::vector<Node*> > m_bounds;
	
	int m_node_id;
	std::map<int, Node*> m_nodes;

	Node *m_last_start, *m_last_end;

	std::vector<ee::Vector> m_routes;

}; // PathVisibleSimple

}
}

#endif // _LR_PREVIEW_PATH_VISIBLE_SIMPLE_H_