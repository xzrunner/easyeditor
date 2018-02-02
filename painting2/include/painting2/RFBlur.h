#pragma once

#include "painting2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace pt2
{

class RFBlur : public RenderFilter
{
public:
	RFBlur() 
		: RenderFilter(pt2::FM_BLUR) 
	{}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFBlur(*this);
	}

	virtual size_t Size() const override { return sizeof(RFBlur); }

}; // RFBlur

}