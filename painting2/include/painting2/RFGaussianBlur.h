#pragma once

#include "painting2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace pt2
{

class RFGaussianBlur : public RenderFilter
{
public:
	RFGaussianBlur() 
		: RenderFilter(pt2::FM_GAUSSIAN_BLUR)
		, m_iterations(9)
	{}

	virtual bool operator == (const RenderFilter& rf) const override {
		return RenderFilter::operator == (rf) &&
			   m_iterations == static_cast<const RFGaussianBlur&>(rf).m_iterations;
	}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFGaussianBlur(*this);
	}

	virtual size_t Size() const override { return sizeof(RFGaussianBlur); }

	int GetIterations() const { return m_iterations; }
	void SetIterations(int iterations) { m_iterations = iterations; }

private:
	int m_iterations;

}; // RFGaussianBlur

}