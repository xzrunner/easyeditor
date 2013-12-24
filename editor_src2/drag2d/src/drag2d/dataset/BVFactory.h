#pragma once
#include "BVTypes.h"

namespace d2d
{
	class AbstractBV;

	class BVFactory
	{
	public:
		static AbstractBV* createBV(BV_TYPE type);

	}; // BVFactory
}

