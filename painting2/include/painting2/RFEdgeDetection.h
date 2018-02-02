#pragma once

#include "painting2/RenderFilter.h"

#include <memmgr/Allocator.h>

namespace pt2
{

class RFEdgeDetection : public RenderFilter
{
public:
	RFEdgeDetection() 
		: RenderFilter(pt2::FM_EDGE_DETECTION) 
		, m_blend(0.5f)
	{}

	virtual bool operator == (const RenderFilter& rf) const override {
		return RenderFilter::operator == (rf) &&
			   m_blend == static_cast<const RFEdgeDetection&>(rf).m_blend;
	}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFEdgeDetection(*this);
	}

	virtual size_t Size() const override { return sizeof(RFEdgeDetection); }

	float GetBlend() const { return m_blend; }
	void SetBlend(float blend) { m_blend = blend; }

private:
	float m_blend;

}; // RFEdgeDetection

}