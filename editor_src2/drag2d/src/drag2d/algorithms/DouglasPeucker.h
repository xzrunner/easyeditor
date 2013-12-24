#pragma once

#include <vector>

#include "common/Vector.h"

namespace d2d
{
	class DouglasPeucker
	{
	public:
		static void implement(const std::vector<Vector>& line, float precision, std::vector<Vector>& dst);

	private:
		static void pointsReduction(const std::vector<Vector>& line, float precision, std::vector<bool>& flag,
			size_t begin, size_t end);

	}; // DouglasPeucker
}

