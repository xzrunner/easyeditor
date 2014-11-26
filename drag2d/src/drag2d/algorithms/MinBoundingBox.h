#ifndef _D2D_MIN_BOUNDING_BOX_H_
#define _D2D_MIN_BOUNDING_BOX_H_

#include "common/Vector.h"

#include <vector>

namespace d2d
{

class MinBoundingBox
{
public:

	// result:
	// 1  2
	// 0  3
	static void Implement(const std::vector<Vector>& points,
		Vector bounding[4]);

private:
	static void TextOtherDir(const std::vector<Vector>& points, 
		const Vector& start, const Vector& end, 
		float& min_area, Vector bounding[4]);
	static void CalculateB(const std::vector<Vector>& points, float k, 
		float& b_min, float& b_max);

}; // MinBoundingBox

}

#endif // _D2D_MIN_BOUNDING_BOX_H_