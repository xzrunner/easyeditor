#ifndef _EASYEDITOR_CONVEX_HULL_H_
#define _EASYEDITOR_CONVEX_HULL_H_

#include "Vector.h"

#include <vector>

namespace ee
{

class ConvexHull
{
public:
	static void Do(const std::vector<Vector>& points, 
		std::vector<Vector>& convex_hull);

}; // ConvexHull

}

#endif // _EASYEDITOR_CONVEX_HULL_H_