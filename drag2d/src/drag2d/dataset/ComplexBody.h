#pragma once

#include "IBody.h"
#include "FixtureData.h"

#include <vector>

namespace d2d
{
	class ComplexBody : public IBody
	{
	public:
		ComplexBody(b2World* world, const std::vector<FixtureDataInfo>& fixtures, 
			float scale = 1.0f);

		virtual void getRect(Rect& rect, const Vector& position, float angle) const;

	}; // ComplexBody
}

