#pragma once

#include "painting2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace pt2
{

class RFBurningMap : public RenderFilter
{
public:
	RFBurningMap() 
		: RenderFilter(pt2::FM_BURNING_MAP) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFBurningMap(*this);
	}

	virtual size_t Size() const override { return sizeof(RFBurningMap); }

}; // RFBurningMap

}