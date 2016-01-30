#ifndef _LR_PREVIEW_PATH_GRID_H_
#define _LR_PREVIEW_PATH_GRID_H_

#include "IPathfinding.h"
#include "PathUtil.h"

#include <drag2d.h>

namespace lr
{
namespace preview
{

class PathGrid : public IPathfinding
{
public:
	PathGrid(const d2d::Rect& region, int row, int col);

	virtual void DisableRegion(const d2d::Sprite* spr, bool disable);

	virtual void QueryRoute(const d2d::Vector& start, const d2d::Vector& end);

	virtual void DebugDraw() const;

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

	class Network : public INetwork
	{
	public:
		Network(const d2d::Rect& region, int row, int col);
		~Network();

		virtual d2d::Vector TransIDToPos(int id) const;

		void SetStatus(const d2d::Rect& region, bool used);

		VisitedNode* QueryRoute(const d2d::Vector& start, const d2d::Vector& end);

		void DebugDraw() const;

	private:
		Node* QueryNode(const d2d::Vector& pos) const;

		void Expand(VisitedNode* node, const d2d::Vector& end);

		void GetConnections(VisitedNode* node, std::vector<Connection>& connections) const;

	private:
		d2d::Rect m_region;

		int m_row, m_col;
		float m_width, m_height;

		Node* m_nodes;

		VisitedList m_visited;
		CandidateList m_candidate;

		// todo
		friend class PathGrid;

	}; // Network

private:
	Network m_nw;

	std::vector<d2d::Vector> m_routes;

}; // PathGrid

}
}

#endif // _LR_PREVIEW_PATH_GRID_H_