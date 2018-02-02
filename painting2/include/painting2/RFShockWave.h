#pragma once

#include "painting2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace pt2
{

class RFShockWave : public RenderFilter
{
public:
	RFShockWave() 
		: RenderFilter(pt2::FM_SHOCK_WAVE) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFShockWave(*this);
	}

	virtual size_t Size() const override { return sizeof(RFShockWave); }

}; // RFShockWave

}