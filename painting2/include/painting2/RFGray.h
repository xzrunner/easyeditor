#pragma once

#include "painting2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace pt2
{

class RFGray : public RenderFilter
{
public:
	RFGray() 
		: RenderFilter(pt2::FM_GRAY) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFGray(*this);
	}

	virtual size_t Size() const override { return sizeof(RFGray); }

}; // RFGray

}