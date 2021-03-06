#ifndef _LR_PREVIEW_PATH_NAV_MESH_H_
#define _LR_PREVIEW_PATH_NAV_MESH_H_

#include "IPathfinding.h"
#include "PathUtil.h"

#include <SM_Rect.h>

#include <vector>
#include <map>

namespace lr
{
namespace preview
{

class PathNavMesh : public IPathfinding, public INetwork
{
public:
	PathNavMesh(const sm::rect& region);

	//
	// interface IPathfinding
	//
	virtual void DisableRegion(const ee::SprConstPtr& spr, bool disable) override;
	virtual void QueryRoute(const sm::vec2& start, const sm::vec2& end) override;
	virtual void DebugDraw() const override;

	//
	// interface INetwork
	//
	virtual sm::vec2 TransIDToPos(int id) const override;

private:
	struct Node
	{
		sm::vec2 bound[3];

		std::vector<Node*> connected;

		bool m_used;

	}; // Node

private:
	sm::rect m_region;

	Node* m_nodes;

	VisitedList m_visited;
	CandidateList m_candidate;

	std::map<const ee::SprPtr, std::vector<sm::vec2> > m_bounds;

	std::vector<sm::vec2> m_tris;

}; // PathNavMesh

}
}

#endif // _LR_PREVIEW_PATH_NAV_MESH_H_