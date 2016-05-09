#ifndef _EASYEDITOR_DOUGLAS_PEUCKER_H_
#define _EASYEDITOR_DOUGLAS_PEUCKER_H_

#include <SM_Vector.h>

#include <vector>

namespace ee
{

class DouglasPeucker
{
public:
	static void Do(const std::vector<sm::vec2>& line, float precision, std::vector<sm::vec2>& dst);

private:
	static void PointsReduction(const std::vector<sm::vec2>& line, float precision, std::vector<bool>& flag,
		size_t begin, size_t end);

}; // DouglasPeucker

}

#endif // _EASYEDITOR_DOUGLAS_PEUCKER_H_