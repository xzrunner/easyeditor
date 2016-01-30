#ifndef _LR_PREVIEW_INTERFACE_PATHFINDING_H_
#define _LR_PREVIEW_INTERFACE_PATHFINDING_H_

#include <drag2d.h>

namespace lr
{
namespace preview
{

class IPathfinding
{
public:
	virtual ~IPathfinding() {}
	virtual void DisableRegion(const d2d::Sprite* spr, bool disable) = 0;	
	virtual void QueryRoute(const d2d::Vector& start, const d2d::Vector& end) = 0;
	virtual void DebugDraw() const = 0;

}; // IPathfinding

}
}

#endif // _LR_PREVIEW_INTERFACE_PATHFINDING_H_