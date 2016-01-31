#ifndef _LR_PREVIEW_PATH_NAV_MASH_H_
#define _LR_PREVIEW_PATH_NAV_MASH_H_

#include "IPathfinding.h"
#include "PathUtil.h"



namespace lr
{
namespace preview
{

class PathNavMesh : public IPathfinding, public INetwork
{
public:
	PathNavMesh(const ee::Rect& region);

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
	struct Node
	{
		ee::Vector bound[3];

		std::vector<Node*> connected;

		bool m_used;

	}; // Node

private:
	ee::Rect m_region;

	Node* m_nodes;

	VisitedList m_visited;
	CandidateList m_candidate;

	std::map<const ee::Sprite*, std::vector<ee::Vector> > m_bounds;

	std::vector<ee::Vector> m_tris;

}; // PathNavMesh

}
}

#endif // _LR_PREVIEW_PATH_NAV_MASH_H_