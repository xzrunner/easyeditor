#pragma once

#include "painting2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace pt2
{

class RFSwirl : public RenderFilter
{
public:
	RFSwirl() 
		: RenderFilter(pt2::FM_SWIRL) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFSwirl(*this);
	}

	virtual size_t Size() const override { return sizeof(RFSwirl); }

}; // RFSwirl

}