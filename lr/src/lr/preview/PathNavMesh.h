#ifndef _LR_PREVIEW_PATH_NAV_MASH_H_
#define _LR_PREVIEW_PATH_NAV_MASH_H_

#include "IPathfinding.h"
#include "PathUtil.h"

#include <drag2d.h>

namespace lr
{
namespace preview
{

class PathNavMesh : public IPathfinding, public INetwork
{
public:
	PathNavMesh(const d2d::Rect& region);

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
	struct Node
	{
		d2d::Vector bound[3];

		std::vector<Node*> connected;

		bool m_used;

	}; // Node

private:
	d2d::Rect m_region;

	Node* m_nodes;

	VisitedList m_visited;
	CandidateList m_candidate;

	std::map<const d2d::ISprite*, std::vector<d2d::Vector> > m_bounds;

	std::vector<d2d::Vector> m_tris;

}; // PathNavMesh

}
}

#endif // _LR_PREVIEW_PATH_NAV_MASH_H_