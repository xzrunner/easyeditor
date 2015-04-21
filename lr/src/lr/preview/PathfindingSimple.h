#ifndef _LR_PREVIEW_PATHFINDING_SIMPLE_H_
#define _LR_PREVIEW_PATHFINDING_SIMPLE_H_

#include <drag2d.h>

namespace lr
{
namespace preview
{

class PathfindingSimple
{
public:
	PathfindingSimple(const d2d::Rect& region, int row, int col);

	void DisableRegion(const d2d::Rect& r, bool disable);

	void DebugDraw() const;

private:
	struct Node
	{
		Node() : m_used(false) {}

		bool m_used;

	}; // Node

	class Network
	{
	public:
		Network(const d2d::Rect& region, int row, int col);
		~Network();

		void SetStatus(const d2d::Rect& region, bool used);

		void DebugDraw() const;

	private:
		d2d::Rect m_region;

		int m_row, m_col;
		float m_width, m_height;

		Node* m_nodes;

	}; // Network

private:
	Network m_nw;

}; // PathfindingSimple

}
}

#endif // _LR_PREVIEW_PATHFINDING_SIMPLE_H_