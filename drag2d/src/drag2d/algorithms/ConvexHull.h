#ifndef _D2D_CONVEX_HULL_H_
#define _D2D_CONVEX_HULL_H_

#include "common/Vector.h"

#include <vector>

namespace d2d
{

class ConvexHull
{
public:
	static void Implement(const std::vector<Vector>& points, 
		std::vector<Vector>& convex_hull);

}; // ConvexHull

}

#endif // _D2D_CONVEX_HULL_H_