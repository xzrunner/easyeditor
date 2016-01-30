#ifndef _EASYEDITOR_DOUGLAS_PEUCKER_H_
#define _EASYEDITOR_DOUGLAS_PEUCKER_H_

#include "Vector.h"

#include <vector>

namespace ee
{

class DouglasPeucker
{
public:
	static void Do(const std::vector<Vector>& line, float precision, std::vector<Vector>& dst);

private:
	static void PointsReduction(const std::vector<Vector>& line, float precision, std::vector<bool>& flag,
		size_t begin, size_t end);

}; // DouglasPeucker

}

#endif // _EASYEDITOR_DOUGLAS_PEUCKER_H_