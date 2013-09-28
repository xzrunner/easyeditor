#pragma once

#include <vector>

#include "common/Vector.h"

namespace d2d
{
	class CosineSmooth
	{
	public:
		static void implement(const std::vector<Vector>& src, float samplingWidth,
			std::vector<Vector>& dst);

	}; // CosineSmooth
}

