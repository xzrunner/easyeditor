#include "CosineSmooth.h"

#include <cmath>

#include "common/Math.h"

namespace d2d
{

void CosineSmooth::implement(const std::vector<Vector>& src, float samplingWidth, 
							 std::vector<Vector>& dst)
{
	dst.clear();

	if (src.size() < 2) return;

	for (size_t i = 0, n = src.size() - 1; i < n; ++i)
	{
		const Vector& p0 = src[i],
			p1 = src[i+1];
		const size_t samplingCount = static_cast<size_t>(std::floor(std::abs(p1.x - p0.x) / samplingWidth));
		const float dx = (p1.x - p0.x) / samplingCount;
		const float da = PI / samplingCount;
		const float yMid = (p0.y + p1.y) * 0.5f;
		const float ampl = (p0.y - p1.y) * 0.5f;
		dst.push_back(p0);
		for (size_t j = 1; j < samplingCount; ++j)
		{
			Vector pt;
			pt.x = p0.x + j * dx;
			pt.y = yMid + ampl * cos(da * j);
			dst.push_back(pt);
		}
	}
	dst.push_back(src.back());
}

} // d2d