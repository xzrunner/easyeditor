#pragma once

#include "painting2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace pt2
{

class RFRelief : public RenderFilter
{
public:
	RFRelief() 
		: RenderFilter(pt2::FM_RELIEF) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFRelief(*this);
	}

	virtual size_t Size() const override { return sizeof(RFRelief); }

}; // RFRelief

}