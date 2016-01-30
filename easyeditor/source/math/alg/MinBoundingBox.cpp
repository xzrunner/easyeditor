#include "MinBoundingBox.h"
#include "ConvexHull.h"
#include "Math2D.h"

#include <assert.h>

namespace ee
{

// bounding:
// 1  2
// 0  3
bool MinBoundingBox::Do(const std::vector<Vector>& points, 
							   Vector bounding[4])
{
	// convex hull
	std::vector<Vector> convex_hull;
	ConvexHull::Do(points, convex_hull);

	// normal
	float xmin = FLT_MAX, xmax = -FLT_MAX,
		  ymin = FLT_MAX, ymax = -FLT_MAX;
	for (int i = 0, n = convex_hull.size(); i < n; ++i) {
		const Vector& p = convex_hull[i];
		if (p.x < xmin) { xmin = p.x; }
		if (p.x > xmax) { xmax = p.x; }
		if (p.y < ymin) { ymin = p.y; }
		if (p.y > ymax) { ymax = p.y; }
	}

	float area_min = (xmax - xmin) * (ymax - ymin);
	bounding[0].Set(xmin, ymin);
	bounding[1].Set(xmin, ymax);
	bounding[2].Set(xmax, ymax);
	bounding[3].Set(xmax, ymin);

	// other dir
	bool is_other_dir = false;
	for (int i = 0, n = convex_hull.size()-1; i < n; ++i) {
		bool b = TextOtherDir(convex_hull, convex_hull[i], convex_hull[i+1], area_min, bounding);
		if (b) {
			is_other_dir = b;
		}
	}
	bool b = TextOtherDir(convex_hull, convex_hull[0], convex_hull[convex_hull.size()-1], area_min, bounding);
	if (b) {
		is_other_dir = b;
	}

	assert(Math2D::IsTurnRight(bounding[0], bounding[1], bounding[2]));

	return b;
}

bool MinBoundingBox::TextOtherDir(const std::vector<Vector>& points, 
								  const Vector& start, const Vector& end,
								  float& min_area, Vector bounding[4])
{
	float dx = start.x - end.x,
		  dy = start.y - end.y;
	if (fabs(dx) < FLT_EPSILON ||
		fabs(dy) < FLT_EPSILON) {
		return false;
	}

	float k0 = dy / dx;
	float b0_min, b0_max;
	CalculateB(points, k0, b0_min, b0_max);
	float e0 = (b0_max - b0_min) / sqrt(1+k0*k0);

	float k1 = -1 / k0;
	float b1_min, b1_max;
	CalculateB(points, k1, b1_min, b1_max);
	float e1 = (b1_max - b1_min) / sqrt(1+k1*k1);
	
	float area = e0 * e1;
	if (area < min_area) {
		min_area = area;
		if (k0 > 0) {
			bounding[0].x = (b1_min - b0_min) / (k0 - k1);
			bounding[1].x = (b1_min - b0_max) / (k0 - k1);
			bounding[2].x = (b1_max - b0_max) / (k0 - k1);
			bounding[3].x = (b1_max - b0_min) / (k0 - k1);
		} else {
			bounding[0].x = (b1_max - b0_min) / (k0 - k1);
			bounding[1].x = (b1_max - b0_max) / (k0 - k1);
			bounding[2].x = (b1_min - b0_max) / (k0 - k1);
			bounding[3].x = (b1_min - b0_min) / (k0 - k1);
		}
		bounding[0].y = k0 * bounding[0].x + b0_min;
		bounding[1].y = k0 * bounding[1].x + b0_max;
		bounding[2].y = k0 * bounding[2].x + b0_max;
		bounding[3].y = k0 * bounding[3].x + b0_min;
	}

	return true;
}

void MinBoundingBox::CalculateB(const std::vector<Vector>& points, float k,
								float& b_min, float& b_max)
{
	b_min = FLT_MAX;
	b_max = -FLT_MAX;
	for (int i = 0, n = points.size(); i < n; ++i) {
		float b = points[i].y - k * points[i].x;
		if (b < b_min) { b_min = b; }
		if (b > b_max) { b_max = b; }
	}
}

}