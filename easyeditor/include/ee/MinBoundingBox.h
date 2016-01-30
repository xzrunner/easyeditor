#ifndef _EASYEDITOR_MIN_BOUNDING_BOX_H_
#define _EASYEDITOR_MIN_BOUNDING_BOX_H_

#include "Vector.h"

#include <vector>

namespace ee
{

class MinBoundingBox
{
public:

	// bounding:
	// 1  2
	// 0  3
	static bool Do(const std::vector<Vector>& points,
		Vector bounding[4]);

private:
	static bool TextOtherDir(const std::vector<Vector>& points, 
		const Vector& start, const Vector& end, 
		float& min_area, Vector bounding[4]);
	static void CalculateB(const std::vector<Vector>& points, float k, 
		float& b_min, float& b_max);

}; // MinBoundingBox

}

#endif // _EASYEDITOR_MIN_BOUNDING_BOX_H_