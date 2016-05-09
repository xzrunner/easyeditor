#include "ConvexHull.h"
#include "Math2D.h"

#include <algorithm>

namespace ee
{

void ConvexHull::Do(const std::vector<sm::vec2>& points, 
					std::vector<sm::vec2>& convex_hull)
{
	convex_hull.clear();

	if (points.size() == 0) {
		return;
	} else if (points.size() == 1) {
		convex_hull.push_back(points[0]);
		return;
	} else if (points.size() == 2) {
		if (points[0] == points[1]) {
			convex_hull.push_back(points[0]);
		} else {
			convex_hull = points;
		}
		return;
	}

	std::vector<sm::vec2> sorted(points);
	sort(sorted.begin(), sorted.end(), sm::Vector2CmpX());

	std::vector<sm::vec2> upper, lower;
	upper.reserve(points.size());
	lower.reserve(points.size());
	upper.push_back(sorted[0]);
	upper.push_back(sorted[1]);
	lower.push_back(sorted[sorted.size() - 1]);
	lower.push_back(sorted[sorted.size() - 2]);

	for (int i = 2, n = sorted.size(); i < n; ++i)
	{
		upper.push_back(sorted[i]);
		int c = upper.size() - 1;
		while (upper.size() >= 3 && !Math2D::IsTurnRight(upper[c- 2], upper[c - 1], upper[c]))
		{
			upper.erase(----upper.end());
			--c;
		}
	}
	for (int i = sorted.size() - 3; i >= 0; --i)
	{
		lower.push_back(sorted[i]);
		size_t c = lower.size() - 1;
		while (lower.size() >= 3 && !Math2D::IsTurnRight(lower[c- 2], lower[c - 1], lower[c]))
		{
			lower.erase(----lower.end());
			--c;
		}
	}

	convex_hull.reserve(lower.size() + upper.size() - 2);
	copy(upper.begin(), upper.end(), back_inserter(convex_hull));
	if (lower.size() > 2) {
		copy(++lower.begin(), --lower.end(), back_inserter(convex_hull));
	}
}

}