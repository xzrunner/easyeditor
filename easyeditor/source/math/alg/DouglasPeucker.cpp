#include "DouglasPeucker.h"
#include "Math2D.h"

namespace ee
{

void DouglasPeucker::Do(const std::vector<sm::vec2>& line, float precision, std::vector<sm::vec2>& dst)
{
	if (line.empty()) {
		return;
	}

	dst.clear();

	std::vector<bool> flag(line.size(), false);
	PointsReduction(line, precision, flag, 0, line.size() - 1);
	for (size_t i = 0, n = line.size(); i < n; ++i)
		if (flag[i]) dst.push_back(line[i]);
}

void DouglasPeucker::PointsReduction(const std::vector<sm::vec2>& line, float precision, std::vector<bool>& flag,
									 size_t begin, size_t end)
{
	if ( begin > end )
		return;

	if ( begin == end )
	{
		flag[begin] = true;
		return;
	}

	if ( end - begin < 2 )
	{
		flag[begin] = true;
		flag[end] = true;
		return;
	}

	size_t savedPointIndex = 0;
	float max = - FLT_MAX;
	for (size_t i = begin + 1; i < end; ++i)
	{
		float dis = Math2D::GetDisPointToSegment(line[i], line[begin], line[end]);
		if (dis > max)
		{
			max = dis;
			savedPointIndex = i;
		}
	}

	if (max < precision)
	{
		flag[begin] = true;
		flag[end] = true;
	}
	else
	{
		PointsReduction(line, precision, flag, begin, savedPointIndex);
		PointsReduction(line, precision, flag, savedPointIndex, end);
	}
}

}