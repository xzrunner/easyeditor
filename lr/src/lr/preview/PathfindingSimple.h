#ifndef _LR_PREVIEW_PATHFINDING_SIMPLE_H_
#define _LR_PREVIEW_PATHFINDING_SIMPLE_H_

#include <drag2d.h>

#include "PathfindingUtil.h"

namespace lr
{
namespace preview
{

class PathfindingSimple
{
public:
	PathfindingSimple(const d2d::Rect& region, int row, int col);

	void DisableRegion(const d2d::Rect& r, bool disable);

	void QueryRoute(const d2d::Vector& start, const d2d::Vector& end);

	void DebugDraw() const;

private:
	struct Node
	{
		Node() : m_used(false) {}

		int id;

		bool m_used;

	}; // Node

	struct Connection
	{
		Connection(float len, Node* n) 
			: len(len), n(n) {}

		float len;
		Node* n;
	}; // Connection

	class Network
	{
	public:
		Network(const d2d::Rect& region, int row, int col);
		~Network();

		void SetStatus(const d2d::Rect& region, bool used);

		VisitedNode* QueryRoute(const d2d::Vector& start, const d2d::Vector& end);

		void DebugDraw() const;

	private:
		Node* QueryNode(const d2d::Vector& pos) const;

		void Expend(VisitedNode* node);

		void GetConnections(VisitedNode* node, std::vector<Connection>& connections) const;

	private:
		d2d::Rect m_region;

		int m_row, m_col;
		float m_width, m_height;

		Node* m_nodes;

		VisitedList m_visited;
		CandidateList m_candidate;

		// todo
		friend class PathfindingSimple;

	}; // Network

private:
	Network m_nw;

	std::vector<d2d::Vector> m_routes;

}; // PathfindingSimple

}
}

#endif // _LR_PREVIEW_PATHFINDING_SIMPLE_H_