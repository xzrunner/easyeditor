#ifndef _LR_PREVIEW_INTERFACE_PATHFINDING_H_
#define _LR_PREVIEW_INTERFACE_PATHFINDING_H_



namespace lr
{
namespace preview
{

class IPathfinding
{
public:
	virtual ~IPathfinding() {}
	virtual void DisableRegion(const ee::Sprite* spr, bool disable) = 0;	
	virtual void QueryRoute(const ee::Vector& start, const ee::Vector& end) = 0;
	virtual void DebugDraw() const = 0;

}; // IPathfinding

}
}

#endif // _LR_PREVIEW_INTERFACE_PATHFINDING_H_