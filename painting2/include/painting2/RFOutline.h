#pragma once

#include "painting2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace pt2
{

class RFOutline : public RenderFilter
{
public:
	RFOutline() 
		: RenderFilter(pt2::FM_OUTLINE) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFOutline(*this);
	}

	virtual size_t Size() const override { return sizeof(RFOutline); }

}; // RFOutline

}