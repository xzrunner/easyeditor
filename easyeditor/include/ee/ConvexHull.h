#ifndef _EASYEDITOR_CONVEX_HULL_H_
#define _EASYEDITOR_CONVEX_HULL_H_

#include <SM_Vector.h>

#include <vector>

namespace ee
{

class ConvexHull
{
public:
	static void Do(const std::vector<sm::vec2>& points, 
		std::vector<sm::vec2>& convex_hull);

}; // ConvexHull

}

#endif // _EASYEDITOR_CONVEX_HULL_H_